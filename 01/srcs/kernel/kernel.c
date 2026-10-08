#include "../drivers/display.h"
#include "../cpu/isr.h"
#include "./gdt.h"

void init_term() {
	clear_term();
}

void kernel_main(void) {
	setup_gdt();
	isr_install();
	init_term();
	// asm volatile ("int $01");

	uint32_t offset = print_str_at("eulmanOS >> ", 0, 0, (vga_color_t)MAGENTA, (vga_color_t)GREEN);
	set_cursor(offset * 2);
}
