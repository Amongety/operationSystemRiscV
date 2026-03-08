#include "../../include/kernelSpace/libc/string.h"

void *memset(void *s, int c, unsigned long n) {
    uint8_t *p = (uint8_t *) s;
    
    while (n--) *(p++) = c;

    return s;
}

void *memcpy(void *dst, const void *src, unsigned long n) {
    uint8_t *d = (uint8_t *) dst;
    const uint8_t *s = (const uint8_t *) src;

    while (n--) *(d++) = *(s++);

    return dst;
}

uint64_t strlen (const char *str) {
    if (str == NULL) return 0;

    const char *p = str;
    while (*p != '\0') p++; 

    return p - str;
}

int strcmp(const char* string1, const char* string2) {	
	while(*string1 && *string1 == *string2) {
		++string1;
		++string2;
	}

	return *(unsigned char*)string1 - *(unsigned char*)string2;
}

int strncmp(const char *s1, const char *s2, uint64_t n) {
	while(n && *s1 && (*s1 == *s2)) {
		++s1; ++s2; --n;
	}

	if(n == 0) return 0;

	return *s1 - *s2;
}

