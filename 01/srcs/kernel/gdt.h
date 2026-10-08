#pragma once
#include "../../includes/stdint.h"

extern void activate_segments();

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));


struct gdt_ptr {
    unsigned short limit; // Taille totale de la GDT - 1
    unsigned int base;    // Adresse mémoire de la table
} __attribute__((packed));

void setup_gdt();
