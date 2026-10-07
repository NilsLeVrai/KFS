#include "../drivers/display.h"
#include "../cpu/isr.h"

void init_term() {
	clear_term();
}

void kernel_main(void) {
	isr_install();
	init_term();
	int i = 0;
	int j = 0;
	int lol = i / j;
	uint32_t offset = print_str_at("eulmanOS >> ", 0, 0, (vga_color_t)MAGENTA, (vga_color_t)GREEN);
	set_cursor(offset * 2);
}
