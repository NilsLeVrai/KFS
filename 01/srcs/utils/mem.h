#pragma once
#include "stdint.h"

void*	kmemmove(void* dest, const void* src, uint32_t n);
void*	kmemcpy(void* dest, const void* src, uint32_t n);
void*	kmemset(void *s, int32_t c, uint32_t n);
void*	kmemchr(const void *s, int c, uint32_t n);
