#include "shell.h"

void	init_shell() {
	uint32_t offset = print_str_at("eulmanOS >> ", 0, 0, (vga_color_t)MAGENTA, (vga_color_t)GREEN);
	set_cursor(offset * 2);
}
