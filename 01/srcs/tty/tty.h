/*
 * tty/tty.h
 * Terminal virtuel : chaque tty_t a son buffer écran, curseur, couleurs.
 * API d'écriture (putchar, write, backspace, clear) sur un tty donné.
 */

#pragma once

#include "../utils/stdint.h"
#include "../drivers/vga.h"
#include "../utils/circular_buffer.h"

typedef struct tty_s {
	uint16_t	buffer[VGA_ROW * VGA_COL];
	uint16_t	cur_pos;
	uint32_t	x, y;
	uint8_t		bg_color_default;
	uint8_t		fg_color_default;
} tty_t;

void	tty_putchar(tty_t* t, int8_t c, bool custom_color, uint8_t fg, uint8_t bg);
void	tty_write(tty_t* t, const int8_t* s, bool custom_color, uint16_t fg, uint8_t bg);
void	tty_clear(tty_t* t);
void	tty_backspace(tty_t* t);
