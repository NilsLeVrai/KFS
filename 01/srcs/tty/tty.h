/*
 * tty/tty.h
 * Terminal virtuel : chaque tty_t a son historique (anneau de lignes),
 * sa fenêtre d'affichage, son curseur, ses couleurs et sa ligne en cours.
 * API d'écriture (putchar, write, backspace) et de défilement.
 */

#pragma once

#include "../utils/stdint.h"
#include "../drivers/vga.h"

#define TTY_LINE_MAX 64
#define TTY_HIST_ROWS 200

typedef struct tty_s {
	uint16_t	hist[TTY_HIST_ROWS][VGA_COL];
	uint32_t	head;		// index dans hist de la ligne 0 de l'écran live
	uint32_t	filled;		// lignes d'historique dispo au-dessus de l'écran
	uint32_t	view;		// lignes remontées (0 = live)
	uint32_t	x, y;		// curseur, relatif à l'écran live
	uint8_t		bg_color_default;
	uint8_t		fg_color_default;
	int8_t		line[TTY_LINE_MAX];
	uint32_t	line_len;
} tty_t;

int32_t	tty_putchar(tty_t* t, int8_t c, bool custom_color, uint8_t fg, uint8_t bg);
int32_t	tty_write(tty_t* t, const int8_t* s, bool custom_color, uint8_t fg, uint8_t bg);
void	tty_backspace(tty_t* t);
void	tty_sync(tty_t* t);
void	tty_init_one(tty_t* t, uint8_t fg, uint8_t bg);
void	tty_view_up(tty_t* t, uint32_t n);
void	tty_view_down(tty_t* t, uint32_t n);
