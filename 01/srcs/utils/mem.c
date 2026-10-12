#include "mem.h"
#include "stdint.h"


void*	kmemcpy(void* dest, const void* src, uint32_t n) {
	int8_t*			p_dest;
	const int8_t*	p_src;

	if (!dest && !src) {return ((void *)0);}
	p_dest = dest;
	p_src = src;
	while (n--) {
		*p_dest = *p_src;
		p_dest++;
		p_src++;
	}
	return (dest);
}

void*	kmemmove(void* dest, const void* src, uint32_t n) {
	if (dest == src) {
		return dest;
	}
	else if (dest < src) {
		kmemcpy(dest, src, n);
	}
	else {
		while (n--) {
			((uint8_t*)dest)[n] = ((uint8_t*)src)[n];
		}
	}
	return (dest);
}

void*	kmemset(void*	s, int32_t c, uint32_t n)
{
	uint8_t*	p;

	p = s;
	while (n--) {
		*p++ = (uint8_t)c;
	}
	return (s);
}

int	kmemcmp(const void *s1, const void *s2, uint32_t n)
{
    const uint8_t *a = s1;
    const uint8_t *b = s2;

    while (n--) {
        if (*a != *b) {
            return *a - *b;
		}
        a++;
        b++;
    }
    return 0;
}

void	*kmemchr(const void *s, int c, uint32_t n)
{
	const uint8_t	*p_src;
	uint8_t		ch;

	p_src = s;
	ch = c;
	while (n--) {
		if (*p_src == ch) {
			return ((void *)p_src);
		}
		p_src++;
	}
	return ((void *)0);
}
