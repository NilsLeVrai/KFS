#pragma once
#include "../cpu/isr.h"
#include "../drivers/ports.h"
#include "../drivers/display.h"

void irq1_handler(registers_t* regs);
void keyboard_init();
void keyboard_poll();
void keyboard_getchar();
