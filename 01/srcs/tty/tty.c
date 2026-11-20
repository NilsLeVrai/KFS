/*
 * tty/tty.c
 * L'historique du tty (anneau de TTY_HIST_ROWS lignes) fait foi ;
 * la VGA n'est que le miroir de la fenêtre affichée du tty actif.
 * Écriture : 1 cellule dans l'historique (+ 1 en VGA si actif et en bas).
 * Redessin complet (tty_sync) seulement au scroll / switch / page up-down.
 * Curseur matériel mis à jour uniquement pour le tty actif.
 */

#include "tty.h"
#include "tty_manager.h"
#include "../drivers/vga.h"
#include "../utils/mem.h"

volatile uint16_t		*vga_ptr = (uint16_t *)VGA_ADRESS;

static inline uint16_t tty_blank(tty_t* t) {
	return vga_create_char(' ', vga_create_color(t->fg_color_default, t->bg_color_default));
}

// ligne y de l'écran, décalée de `back` lignes vers le passé
static inline uint16_t* tty_row(tty_t* t, uint32_t y, uint32_t back) {
	return t->hist[(t->head + TTY_HIST_ROWS + y - back) % TTY_HIST_ROWS];
}

static void tty_clear_row(tty_t* t, uint16_t* row) {
	uint16_t b = tty_blank(t);
	for (uint32_t i = 0; i < VGA_COL; i++) {
		row[i] = b;
	}
}

void tty_init_one(tty_t* t, uint8_t fg, uint8_t bg) {
	t->fg_color_default = fg;
	t->bg_color_default = bg;
	t->head = t->filled = t->view = 0;
	t->x = t->y = 0;
	t->line_len = 0;
	for (uint32_t r = 0; r < TTY_HIST_ROWS; r++) {
		tty_clear_row(t, t->hist[r]);
	}
}

static void	tty_sync_cursor(tty_t* t) {
	if (t != tty_current())
		return;
	if (t->view == 0)
		vga_set_cursor(t->x, t->y);
	else
		vga_set_cursor(0, VGA_ROW);     // hors écran = curseur caché
}

void	tty_sync(tty_t* t) {
	if (t != tty_current())
		return;
	for (uint32_t y = 0; y < VGA_ROW; y++) {
		kmemcpy((void*)&vga_ptr[y * VGA_COL], tty_row(t, y, t->view), VGA_COL * sizeof(uint16_t));
	}
	tty_sync_cursor(t);
}

static void tty_scroll(tty_t *t) {
	t->head = (t->head + 1) % TTY_HIST_ROWS;
	// la nouvelle ligne du bas écrase la plus vieille ligne de l'anneau
	tty_clear_row(t, tty_row(t, VGA_ROW - 1, 0));
	if (t->filled < TTY_HIST_ROWS - VGA_ROW) {
		t->filled++;
	}
	tty_sync(t);
}

// si on écrit alors qu'on est remonté, on revient en bas
static void tty_snap_live(tty_t* t) {
	if (t->view) {
		t->view = 0;
		tty_sync(t);
	}
}

static void	tty_set_cell(tty_t* t, uint32_t x, uint32_t y, uint16_t entry) {
	tty_row(t, y, 0)[x] = entry;
	if (t == tty_current() && t->view == 0) {
		vga_ptr[y * VGA_COL + x] = entry;
	}
}

void tty_view_up(tty_t* t, uint32_t n) {
	t->view = (t->view + n > t->filled) ? t->filled : t->view + n;
	tty_sync(t);
}

void tty_view_down(tty_t* t, uint32_t n) {
	t->view = (n > t->view) ? 0 : t->view - n;
	tty_sync(t);
}

int32_t	tty_putchar(tty_t* t, int8_t c, bool custom_color, uint8_t fg, uint8_t bg) {
	int32_t	cnt = 0;

	tty_snap_live(t);
	if (c == '\n') {
		t->x = 0;
		t->y++;
	} else {
		uint8_t color = vga_create_color(custom_color ? fg : t->fg_color_default,
		                                 custom_color ? bg : t->bg_color_default);
		tty_set_cell(t, t->x, t->y, vga_create_char(c, color));
		cnt++;
		if (++t->x >= VGA_COL) {
			t->x = 0;
			t->y++;
		}
	}
	if (t->y >= VGA_ROW) {
		t->y = VGA_ROW - 1;
		tty_scroll(t);              // tty_sync remet aussi le curseur
	} else {
		tty_sync_cursor(t);
	}
	return (cnt);
}

int32_t	tty_write(tty_t* t, const int8_t* s, bool custom_color, uint8_t fg, uint8_t bg) {
	int32_t cnt = 0;
	while (*s) {
		cnt += tty_putchar(t, *s++, custom_color, fg, bg);
	}
	return (cnt);
}

void	tty_backspace(tty_t* t) {
	tty_snap_live(t);
	if (t->x > 0) {
		t->x--;
	} else if (t->y > 0) {
		t->y--;
		t->x = VGA_COL - 1;
	} else {
		return ;
	}
	tty_set_cell(t, t->x, t->y, tty_blank(t));
	tty_sync_cursor(t);
}
