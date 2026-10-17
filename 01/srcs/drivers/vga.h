/*
 * drivers/vga.h
 * Driver VGA mode texte 80x25 (0xB8000).
 * Couleurs, cellule = [couleur:8][char:8], curseur matériel (index y*80+x).
 * Ne connaît pas les terminaux : c'est tty/ qui décide quoi afficher.
 */

#pragma once
#include "../utils/stdint.h"

#define VGA_ADRESS 0xB8000
#define VGA_ROW 25
#define VGA_COL 80
#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

typedef enum __attribute__ ((__packed__)) e_vga_color {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GREY = 7,
	DARK_GREY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	YELLOW = 14,
	WHITE = 15
} vga_color_t;

void		set_cursor(int offset);
void vga_set_cursor(uint32_t x, uint32_t y);
int			get_cursor();
uint8_t		vga_create_color(uint8_t fg, uint8_t bg);
uint16_t	vga_create_char(uint8_t c, uint8_t color);
uint8_t		vga_extract_color(uint16_t vga_elem);
void		clear_term();
void		print_char_at(uint8_t c, uint32_t x, uint32_t y, uint8_t fg, uint8_t bg);
uint32_t	print_str_at(char* str, uint32_t x, uint32_t y, uint8_t fg, uint8_t bg);
void		kputchar(char c, uint8_t fg, uint8_t bg);
