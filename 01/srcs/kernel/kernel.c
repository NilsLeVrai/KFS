#include "../drivers/display.h"
#include "../cpu/isr.h"
#include "./gdt.h"
#include "../shell/shell.h"
#include "../drivers/ports.h"
#include "../drivers/keyboard.h"

void init_term() {
	clear_term();
}

void user_test() {
}

void kernel_main() {
	setup_gdt();
	isr_install();
	while (port_byte_in(0x64) & 1)
		port_byte_in(0x60);
	asm volatile("sti");
	keyboard_init();

	init_term();
	init_shell();
	// asm volatile ("int $00");
	// switch_to_user_mode();

	for (;;) asm volatile("hlt");
}
