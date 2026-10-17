/*
 * tty/tty_manager.h
 * Possède les TERMINAL_NB terminaux et l'index du tty actif.
 */

#pragma once
#include "tty.h"

#define	TERMINAL_NB 1

tty_t			terms[TERMINAL_NB];
static uint8_t	current_term_id = 0;

void	tty_init();
tty_t*	tty_current();
void	tty_switch(uint32_t idx);
