/*
 * tty/tty_manager.c
 * tty_init initialise les terminaux, tty_current renvoie celui affiché,
 * tty_switch change de terminal (recopie son buffer en VGA).
 */

#include "tty_manager.h"
#include "../utils/mem.h"

void	tty_init() {
	terms[0].bg_color_default = 0;
	terms[0].fg_color_default = 1;
	terms[0].x = 0;
	terms[0].y = 0;
	kmemset(terms[0].buffer, 0, VGA_COL * VGA_ROW * 2);
}

tty_t*	tty_current() {
	return (&terms[current_term_id]);
}

void	tty_switch(uint32_t idx) {

}
