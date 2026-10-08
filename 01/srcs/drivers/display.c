#include "display.h"
#include "ports.h"

uint16_t *vga_adr_ptr = (uint16_t *)VGA_ADRESS;

void set_cursor(int offset) {
    offset /= 2;
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

static inline uint8_t create_vga_color(uint8_t fg, uint8_t bg) {
	return (fg | bg << 4);
}

static inline uint16_t create_vga_char(uint8_t c, uint8_t color) {
	return ((uint16_t)c | (uint16_t)color << 8);
}

void clear_term() {
	int count = 0;
	for (int i = 0; i < VGA_COL; i++) {
		for (int j = 0; j < VGA_ROW; j++) {
			*(vga_adr_ptr + count) = create_vga_char('x', create_vga_color((vga_color_t)BLACK, (vga_color_t)BLACK));
			count ++;
		}
	}
	set_cursor(0);
}

void	print_char_at(uint8_t c, uint32_t x, uint32_t y, uint8_t fg, uint8_t bg) {
	*(vga_adr_ptr + ((VGA_COL * y) + x)) = create_vga_char(c, create_vga_color(fg, bg));
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
