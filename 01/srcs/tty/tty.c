/*
 * tty/tty.c
 * Le buffer du tty fait foi ; la VGA n'est que le miroir du tty actif.
 * Écriture : 1 cellule dans le buffer (+ 1 en VGA si actif).
 * Redessin complet (tty_sync) seulement au scroll / clear / switch.
 * Curseur matériel mis à jour uniquement pour le tty actif.
 */

#include "tty.h"
#include "tty_manager.h"
#include "../drivers/vga.h"
#include "../utils/mem.h"

#define pos_to_linear(x, y) (y) * VGA_COL + (x)

volatile uint16_t		*vga_ptr = (uint16_t *)VGA_ADRESS;

static void	tty_set_cell(tty_t* t, uint32_t x, uint32_t y, uint16_t entry) {
	uint32_t i = pos_to_linear(x, y);
	t->buffer[i] = entry;
	if (t == tty_current()) {
		vga_ptr[i] = entry;
	}
}

static void	tty_sync(tty_t* t) {
	if (t == tty_current()) {
		kmemcpy((void*)vga_ptr, t->buffer, sizeof(t->buffer));
		vga_set_cursor(t->x, t->y);
	}
}

static void tty_scroll(tty_t *t) {
	// kmemmove le buffer et tty_sync
}

static void	tty_sync_cursor(tty_t* t) {
	if (t == tty_current()) {
		vga_set_cursor(t->x, t->y);
	}
}

void	tty_putchar(tty_t* t, int8_t c, bool custom_color, uint8_t fg, uint8_t bg) {
	if (c == '\n') {
		t->x = 0;
		t->y++;
	} else {
		tty_set_cell(t, t->x, t->y, vga_create_char(c, vga_create_color((custom_color ? fg : t->fg_color_default), (custom_color ? bg : t->bg_color_default))));
		if (++t->x + 1 >= VGA_COL) {
			t->x = 0;
			t->y++;
		}
		tty_sync_cursor(t);
	}
}

void	tty_write(tty_t* t, const int8_t* s, bool custom_color, uint16_t fg, uint8_t bg) {
	while (*s) {
		tty_putchar(t, *s++, custom_color, fg, bg);
	}
	if (t == tty_current()) {
		vga_set_cursor(t->x, t->y);
	}
}

void	tty_backspace(tty_t* t) {
	if (t->x > 0) {
		t->x--;
	} else if (t->y > 0) {
		t->y--;
		t->x = VGA_COL - 1;
	} else {
		return ;
	}
	uint32_t i = pos_to_linear(t->x, t->y);
	tty_set_cell(t, t->x, t->y, vga_create_char(' ', vga_create_color(t->fg_color_default, t->bg_color_default)));
	tty_sync_cursor(t);
}

void	tty_clear(tty_t* t) {

}
