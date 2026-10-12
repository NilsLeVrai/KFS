#pragma once
#include "stdint.h"
#include "../drivers/keyboard.h"
#include "../drivers/display.h"

uint32_t kreadline(char *out, uint32_t max) {
	// uint32_t n = 0;
	// for (;;) {
	// 	char c = keyboard_getchar();
	// 	if (c == '\n') { putchar('\n'); out[n] = 0; return n; }
	// 	if (c == '\b') {
	// 		if (n > 0) { n--; putchar('\b'); putchar(' '); putchar('\b'); }
	// 		continue;
	// 	}
	// 	if (n < max - 1) { out[n++] = c; putchar(c); }
	// }
	return 0;
}
