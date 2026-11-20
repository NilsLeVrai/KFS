/*
 * tty/tty_manager.h
 * Possède les TERMINAL_NB terminaux et l'index du tty actif.
 */

#pragma once
#include "tty.h"

#define	TERMINAL_NB 3

void	tty_init();
tty_t*	tty_current();
void	tty_switch(uint32_t idx);
void	tty_next(void);
void	tty_prev(void);
tty_t*  tty_get(uint32_t idx);
void	tty_page_up(void);
void	tty_page_down(void);
