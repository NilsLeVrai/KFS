/*
 * drivers/keyboard.h
 * Driver clavier PS/2 : produit des caractères, n'affiche rien.
 * L'écho et l'édition de ligne sont dans kreadline.
 */

#pragma once
#include "../cpu/isr.h"
#include "../drivers/ports.h"

void irq1_handler(registers_t* regs);
void	kbd_init();
int32_t	kbd_poll();
int8_t	kbd_getchar();
