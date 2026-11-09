#include "builtins.h"

void	kreboot() {
	asm volatile ("cli");
	struct __attribute__((packed)) { uint16_t limit; uint32_t base; } idt = {0, 0};
	asm volatile ("lidt %0" :: "m"(idt));
	asm volatile ("int $0");
}
