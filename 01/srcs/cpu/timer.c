#include "timer.h"


/* oui, on aurait pu mettre 1000 */

#define CLOCK_FREQUENCY 1193182 //cristal de mes couilles
#define DIVISOR 0.000838095

static volatile uint32_t counter = 0;

//static void counter

void ksleep(uint32_t ms) {
    uint32_t start = counter + ms;
    
    while (counter < start);
}

static void irq0_handler(registers_t* regs) {
    counter++;
}

void timer_init() {
    uint32_t divisor = CLOCK_FREQUENCY * DIVISOR;
    port_byte_out(0x43, 0x36);
    port_byte_out(0x40, divisor & 0xFF);
    port_byte_out(0x40, divisor >> 8);
    register_interrupt_handler(IRQ0, irq0_handler);
}

/* void sleep() {

} */

