#include "../../include/kernelSpace/libc/string.h"

void *memset(void *s, int c, unsigned long n) {
    uint8_t *p = (uint8_t *) s;
    while (n--)
        *(p++) = c;
    return s;
}

void *memcpy(void *dst, const void *src, unsigned long n) {
    uint8_t *d = (uint8_t *) dst;
    const uint8_t *s = (const uint8_t *) src;
    while (n--)
        *d++ = *s++;
    return dst;
}

