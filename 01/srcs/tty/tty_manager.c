/*
 * tty/tty_manager.c
 * tty_init initialise les terminaux, tty_current renvoie celui affiché,
 * tty_switch change de terminal (recopie son historique en VGA),
 * tty_page_up / tty_page_down font défiler le tty actif.
 */

#include "tty_manager.h"
#include "tty.h"

tty_t			terms[TERMINAL_NB];
static uint8_t	current_term_id = 0;

void	tty_init() {
	static const uint8_t fg[TERMINAL_NB] = {1, 6, 4};
	static const uint8_t bg[TERMINAL_NB] = {0, 5, 3};

	for (uint32_t t = 0; t < TERMINAL_NB; t++) {
		tty_init_one(&terms[t], fg[t], bg[t]);
	}
	tty_sync(tty_current());
}

tty_t*	tty_current() {
	return (&terms[current_term_id]);
}

void	tty_switch(uint32_t idx) {
	if (idx >= TERMINAL_NB || idx == current_term_id) {
		return;
	}
	current_term_id = idx;
	tty_sync(&terms[idx]);
}

void	tty_next(void) {
	tty_switch((current_term_id + 1) % TERMINAL_NB);
}

void	tty_prev(void) {
	tty_switch((current_term_id + TERMINAL_NB - 1) % TERMINAL_NB);
}

tty_t*  tty_get(uint32_t idx) {
	return (idx < TERMINAL_NB ? &terms[idx] : 0);
}

void tty_page_up(void) {
	tty_view_up(tty_current(), VGA_ROW - 1);
}

void tty_page_down(void) {
	tty_view_down(tty_current(), VGA_ROW - 1);
}
