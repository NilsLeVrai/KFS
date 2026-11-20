/*
 * drivers/keyboard.c
 * IRQ1 : stocke le scancode brut dans un ring buffer, rien d'autre.
 * Hors interruption : kbd_poll décode (QWERTY, shift), kbd_getchar bloque
 * (hlt) jusqu'à une touche.
 */

#include "keyboard.h"

#define KEYBOARD_BUFFER_SIZE 128
static volatile uint8_t		buf[KEYBOARD_BUFFER_SIZE];
static volatile uint32_t	head = 0;
static volatile uint32_t	tail = 0;
static int32_t				shift = 0;
#define KBD_SHORTCUT_HANDLERS_SIZE 128

#define KBD_MAX_SHORTCUTS 32
static kbd_shortcut_t shortcuts[KBD_MAX_SHORTCUTS];
static uint32_t       shortcut_count = 0;
static uint8_t        mods = 0;
static uint8_t        ext  = 0;   // on vient de recevoir 0xE0


void	irq1_handler(registers_t* regs) {
	uint8_t scancode = port_byte_in(0x60);
	uint32_t next = (head + 1) & (KEYBOARD_BUFFER_SIZE - 1);
    if (next != tail) {
        buf[head] = scancode;
        head = next;
    }
}

void	kbd_init() {
	register_interrupt_handler(IRQ1, irq1_handler);
}

int32_t kbd_register_shortcut(uint8_t m, uint16_t key, kbd_handler_t fn) {
	for (uint32_t i = 0; i < shortcut_count; i++)
		if (shortcuts[i].mods == m && shortcuts[i].key == key)
			return (-1);                       // déjà pris
	if (shortcut_count >= KBD_MAX_SHORTCUTS)
		return (-1);
	shortcuts[shortcut_count++] = (kbd_shortcut_t){ m, key, fn };
	return (0);
}

static int32_t try_shortcut(uint16_t key) {
	for (uint32_t i = 0; i < shortcut_count; i++)
		if (shortcuts[i].key == key && shortcuts[i].mods == mods) {
			shortcuts[i].handler();
			return (1);
		}
	return (0);
}

static const int8_t map[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
    0, '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0, ' ',
};

static const int8_t map_shift[128] = {
    0, 27, '!','@','#','$','%','^','&','*','(',')','_','+', '\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0, 'A','S','D','F','G','H','J','K','L',':','"','~',
    0, '|','Z','X','C','V','B','N','M','<','>','?', 0,
    '*', 0, ' ',
};

static int32_t decode(uint8_t sc) {
	if (sc == 0xE0) { ext = 1; return (-1); }

	uint8_t  release = sc & 0x80;
	uint16_t key     = (ext ? 0x100 : 0) | (sc & 0x7F);
	ext = 0;

	// Modificateurs (gauche et droite)
	uint8_t bit = 0;
	switch (key) {
			case 0x2A: case 0x36:                 bit = KBD_MOD_SHIFT; break;
			case 0x1D: case KEY_EXT(0x1D):        bit = KBD_MOD_CTRL;  break;
			case 0x38: case KEY_EXT(0x38):        bit = KBD_MOD_ALT;   break;
			case KEY_EXT(0x2A): case KEY_EXT(0x36): return (-1); // faux shifts envoyés avec les flèches, on ignore
	}
	if (bit) {
		if (release) mods &= ~bit; else mods |= bit;
		return (-1);
	}
	if (release)
		return (-1);

	if (try_shortcut(key))
		return (-1);          // raccourci consommé, pas de caractère

	if (key & 0x100)
		return (-1);          // touche étendue sans raccourci
	int8_t c = (mods & KBD_MOD_SHIFT) ? map_shift[key] : map[key];
	return (c ? c : -1);
}


int32_t	kbd_poll() {
    while (tail != head) {
        uint8_t sc = buf[tail];
        tail = (tail + 1) & (KEYBOARD_BUFFER_SIZE - 1);
        int32_t c = decode(sc);
        if (c >= 0) return (c);
    }
    return (-1);
}

int8_t	kbd_getchar() {
    int32_t	c;
    for (;;) {
        __asm__ volatile ("cli");
        c = kbd_poll();
        if (c >= 0) { __asm__ volatile ("sti"); return ((int8_t)c); }
        __asm__ volatile ("sti; hlt");
    }
}
