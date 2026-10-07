#include "idt.h"
#include "isr.h"

idt_gate_t		idt[256] = {0};
idt_register_t	idt_reg;

void	set_idt_gate() {
	idt[0].low_offset = (uint16_t)((uint32_t)&isr8 & 0xffff);
	idt[0].selector = 0x08;
	idt[0].always0 = 0;
	idt[0].flags = 0x8E;
	idt[0].high_offset = (uint16_t)((uint32_t)&isr8 >> 16);
}

void	load_idt() {
	idt_reg.base = (uint32_t)&idt;
	idt_reg.limit = 256 * sizeof(idt_gate_t) - 1;
	asm volatile("lidt (%0)" : : "r" (&idt_reg));
}
