/*
 * utils/kreadline.h
 * Lecture d'une ligne au clavier avec écho dans un tty donné.
 */

#pragma once
#include "mem.h"
#include "stdint.h"
#include "../drivers/keyboard.h"
#include "../tty/tty.h"

int8_t*	kreadline(int8_t *out, uint32_t max);
