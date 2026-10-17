/*
 * drivers/ports.h
 * Accès aux ports d'E/S x86 (in/out 8 bits).
 * Brique la plus basse : utilisée par vga, keyboard et le PIC.
 */

#pragma once

unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);
