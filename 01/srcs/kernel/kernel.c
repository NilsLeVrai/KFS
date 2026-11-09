/*
 * kernel/kernel.c
 * kernel_main, appelé par boot.asm.
 * Init : GDT -> IDT/PIC -> purge clavier -> sti -> clavier -> tty,
 * puis boucle prompt / kreadline.
 */

#include "../drivers/vga.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "./gdt.h"
#include "../shell/shell.h"
#include "../drivers/ports.h"
#include "../drivers/keyboard.h"
#include "../utils/kreadline.h"
#include "../tty/tty_manager.h"
#include "../tty/tty.h"
#include "../utils/printk.h"

void user_test() {
}

void kernel_main() {
	setup_gdt();
	isr_install();
	while (port_byte_in(0x64) & 1)
		port_byte_in(0x60);
	asm volatile("sti");
	kbd_init();
	timer_init();

	tty_init();
	shell_init();
	shell_start();


	// asm volatile ("int $00");
	// switch_to_user_mode();

	for (;;) asm volatile("hlt");
}
