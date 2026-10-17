/*
 * utils/kreadline.c
 * Gère la ligne (longueur, backspace limité à la saisie) ;
 * le tty ne fait que l'affichage, le clavier que la lecture.
 */

#include "kreadline.h"
#include "stdint.h"

int8_t*	kreadline(tty_t* t, int8_t *out, uint32_t max, const int8_t* prompt) {
	uint32_t idx = 0;
	for (;;) {
		int8_t c = kbd_getchar();
		if (c == '\n') {
			out[idx] = 0;
			return (out);
		} else if (c == '\b') {
			if (idx > 0) {
				tty_backspace(t);
				idx--;
			}
			continue ;
		}
		tty_putchar(t, c, 0, 0, 0);
		out[idx] = c;
		if (idx == max - 1) {
			out[idx + 1] = 0;
			return (out);
		}
		idx++;
	}
	return (0);
}
