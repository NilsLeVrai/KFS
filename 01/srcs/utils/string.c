#include "string.h"
#include "stdint.h"

uint32_t	kstrlen(int8_t* s) {
	uint32_t i = 0;

	while (*s) {
		s++;
		i++;
	}
	return (i);
}

uint8_t	kstrcmp(int8_t* s1, int8_t* s2) {
	int ret;
	while ((ret = *(uint8_t *) s1 - *(uint8_t *) s2++) == 0 && *s1++);
	return ret;
}

int8_t*		kstrcpy(int8_t* dest, const int8_t* src) {
	while (*src) {
		*dest =  *src;
		dest++;
		src++;
	}
	return (dest);
}
