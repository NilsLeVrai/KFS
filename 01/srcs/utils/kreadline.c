/*
 * utils/kreadline.c
 * Gère la ligne (longueur, backspace limité à la saisie) ;
 * le tty ne fait que l'affichage, le clavier que la lecture.
 */

#include "kreadline.h"
#include "stdint.h"
#include "../tty/tty_manager.h"

int8_t*	kreadline(int8_t *out, uint32_t max) {
	for (;;) {
		int8_t c = kbd_getchar();
		tty_t *t = tty_current();

		if (c == '\n') {
			kmemcpy(out, t->line, t->line_len);
			out[t->line_len] = 0;
			t->line_len = 0;
			return (out);
		}
		if (c == '\b') {
			if (t->line_len > 0) {
				tty_backspace(t);
				t->line_len--;
			}
			continue ;
		}
		if (t->line_len < max - 1 && t->line_len < TTY_LINE_MAX - 1) {
			t->line[t->line_len++] = c;
			tty_putchar(t, c, 0, 0, 0);
		}
	}
}
