/*
 * drivers/keyboard.h
 * Driver clavier PS/2 : produit des caractères, n'affiche rien.
 * L'écho et l'édition de ligne sont dans kreadline.
 */

#pragma once
#include "../cpu/isr.h"
#include "../drivers/ports.h"
#include "../utils/mem.h"

#define KBD_MOD_SHIFT	0x01
#define KBD_MOD_CTRL	0x02
#define KBD_MOD_ALT		0x04

#define KEY_EXT(sc)		(0x100 | (sc))   // touche préfixée E0
#define KEY_RIGHT		KEY_EXT(0x4D)
#define KEY_LEFT		KEY_EXT(0x4B)
#define KEY_UP			KEY_EXT(0x48)
#define KEY_DOWN		KEY_EXT(0x50)
#define KEY_PGUP		KEY_EXT(0x49)
#define KEY_PGDOWN		KEY_EXT(0x51)

typedef void (*kbd_handler_t)(void);

typedef struct kbd_shortcut_s {
        uint8_t       mods;    // combinaison de KBD_MOD_*
        uint16_t      key;     // scancode "make" (+0x100 si E0)
        kbd_handler_t handler;
} kbd_shortcut_t;


void	irq1_handler(registers_t* regs);
void	kbd_init();
int32_t	kbd_poll();
int8_t	kbd_getchar();
void	kbd_register_event(uint8_t scancode, void(*event)());
int32_t	kbd_register_shortcut(uint8_t mods, uint16_t key, kbd_handler_t fn);
