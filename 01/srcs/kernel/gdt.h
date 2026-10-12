#pragma once
#include "../utils/stdint.h"

#define GDT_BASE_ADDR 0x00000800
#define MEMORY_LIMIT_10MB 0x00A00

extern void activate_segments();
extern void	switch_to_user_mode();

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
