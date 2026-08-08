#ifndef __STRING_H__
#define __STRING_H__

#include <stdint.h>
#include <stddef.h>

void *memset(void *s, int c, unsigned long n);
void *memcpy(void *dst, const void *src, unsigned long n);
uint64_t strlen (const char *str);
int strcmp(const char* string1, const char* string2); 
int strncmp(const char *s1, const char *s2, uint64_t n);
char *strchr(const char *s, int c);
char* strcpy(char* dest, const char* src);
char* strrchr(const char* s, int c);

#endif
