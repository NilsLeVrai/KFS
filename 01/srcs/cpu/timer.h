#pragma once
#include "../utils/stdint.h"
#include "../cpu/isr.h"
#include "../drivers/ports.h"
#include "../utils/printk.h"

void timer_init();
void ksleep(uint32_t ms);