/*
 * kernel/kernel.c
 * kernel_main, appelé par boot.asm.
 * Init : GDT -> IDT/PIC -> purge clavier -> sti -> clavier -> tty,
 * puis boucle prompt / kreadline.
 */

#include "../drivers/vga.h"
#include "../cpu/isr.h"
#include "./gdt.h"
#include "../shell/shell.h"
#include "../drivers/ports.h"
#include "../drivers/keyboard.h"
#include "../utils/kreadline.h"
#include "../tty/tty_manager.h"
#include "../tty/tty.h"

void user_test() {
}

void kernel_main() {
	setup_gdt();
	isr_install();
	while (port_byte_in(0x64) & 1)
		port_byte_in(0x60);
	asm volatile("sti");
	kbd_init();

	tty_init();
	tty_write(tty_current(), "eulmanOs >> ", 0, 2, 3);
	for (;;) {
		static int8_t test[64];
		kreadline(tty_current(), test, 64, "neumanOs >> ");
		tty_write(tty_current(), "\n", 0, 0, 0);
	}

	// asm volatile ("int $00");
	// switch_to_user_mode();

	for (;;) asm volatile("hlt");
}
