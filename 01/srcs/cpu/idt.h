/*
 * cpu/idt.h
 * Types de l'IDT (gate, registre IDTR) et API de remplissage / chargement.
 * Ne connaît aucun handler : c'est isr.c qui remplit la table.
 */

#pragma once
#include "../utils/stdint.h"

typedef struct {
    uint16_t	low_offset;
    uint16_t	selector;
    uint8_t		always0;
    uint8_t		flags;
    uint16_t	high_offset;
} __attribute__((packed)) idt_gate_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;


void	set_idt_gate(int n, uint32_t handler);
void	load_idt();
