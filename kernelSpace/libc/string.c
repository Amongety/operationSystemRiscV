#include "../../include/kernelSpace/libc/string.h"

void *memset(void *s, int c, unsigned long n) {
    uint8_t *p = (uint8_t *) s;
    while (n--)
        *(p++) = c;
    return s;
}

