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

static int32_t	decode(uint8_t sc) {
    switch (sc) {
        case 0x2A: case 0x36: shift = 1; return (-1);  // shift press
        case 0xAA: case 0xB6: shift = 0; return (-1);  // shift release
    }
    if (sc & 0x80) return (-1);                        // autres releases
    int8_t c = shift ? map_shift[sc] : map[sc];
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
