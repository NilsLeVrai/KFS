/*
 * drivers/vga.c
 * Implémentation du driver VGA.
 * clear_term / print_str_at / set_cursor(offset) : ancienne API qui écrit
 * directement en VGA, à remplacer par tty/.
 */

#include "vga.h"
#include "ports.h"

volatile uint16_t		*vga_adr_ptr = (uint16_t *)VGA_ADRESS;

void set_cursor(int offset) {
    // offset /= 2;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset >> 8));
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset & 0xff));
}

int get_cursor() {
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = port_byte_in(VGA_DATA_REGISTER) << 8;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += port_byte_in(VGA_DATA_REGISTER);
    return offset * 2;
}

void vga_set_cursor(uint32_t x, uint32_t y) {
	uint16_t pos = y * VGA_COL + x;

	port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
	port_byte_out(VGA_DATA_REGISTER, (uint8_t)(pos >> 8));
	port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
	port_byte_out(VGA_DATA_REGISTER, (uint8_t)(pos & 0xff));
}

uint8_t vga_create_color(uint8_t fg, uint8_t bg) {
	return (fg | bg << 4);
}

uint16_t vga_create_char(uint8_t c, uint8_t color) {
	return ((uint16_t)c | (uint16_t)color << 8);
}

uint8_t vga_extract_color(uint16_t vga_elem) {
	return ((vga_elem >> 8) & 0xff);
}

void clear_term() {
	int count = 0;
	for (int i = 0; i < VGA_COL; i++) {
		for (int j = 0; j < VGA_ROW; j++) {
			*(vga_adr_ptr + count) = vga_create_char('x', vga_create_color((vga_color_t)BLACK, (vga_color_t)BLACK));
			count ++;
		}
	}
	set_cursor(0);
}

void	print_char_at(uint8_t c, uint32_t x, uint32_t y, uint8_t fg, uint8_t bg) {
	*(vga_adr_ptr + ((VGA_COL * y) + x)) = vga_create_char(c, vga_create_color(fg, bg));
}

uint32_t	print_str_at(char* str, uint32_t x, uint32_t y, uint8_t fg, uint8_t bg) {
	uint32_t	xoffset = 0;
	uint32_t	yoffset = 0;
	uint32_t	newline_count = 0;
	while (*str) {
		if (x + xoffset >= VGA_COL) {
			x = 0;
			xoffset = 0;
			newline_count++;
		}
		if (*str == '\n') {
			str++;
			x = 0;
			xoffset = 0;
			newline_count++;
			continue ;
		}
		print_char_at(*str, x + xoffset, y + newline_count, fg, bg);
		xoffset++;
		str++;
	}
	return (VGA_COL * newline_count) + xoffset - 1;
}

void	kputchar(char c, uint8_t fg, uint8_t bg) {
}

void	kputstr(char c, uint8_t fg, uint8_t bg) {
}
