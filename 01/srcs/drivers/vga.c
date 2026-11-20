/*
 * drivers/vga.c
 * Implémentation du driver VGA.
 * clear_term / print_str_at écrivent directement en VGA sans passer par
 * les tty : réservés à l'affichage d'un panic.
 */

#include "vga.h"
#include "ports.h"

volatile uint16_t		*vga_adr_ptr = (uint16_t *)VGA_ADRESS;

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

void clear_term() {
	uint16_t blank = vga_create_char(' ', vga_create_color(BLACK, BLACK));
	for (uint32_t i = 0; i < VGA_COL * VGA_ROW; i++) {
		vga_adr_ptr[i] = blank;
	}
	vga_set_cursor(0, 0);
}

static void	print_char_at(uint8_t c, uint32_t x, uint32_t y, uint8_t fg, uint8_t bg) {
	*(vga_adr_ptr + ((VGA_COL * y) + x)) = vga_create_char(c, vga_create_color(fg, bg));
}

uint32_t	print_str_at(char* str, uint32_t x, uint32_t y, uint8_t fg, uint8_t bg) {
	uint32_t	xoffset = 0;
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
