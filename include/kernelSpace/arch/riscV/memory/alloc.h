#ifndef __ALLOC_H__
#define __ALLOC_H__

#include "../../../libsbi/sci.h"

#define PAGE_SIZE 4096

extern unsigned char _free_ram_start[];
extern unsigned char _free_ram_end[];

unsigned char* alloc_pages(uint32_t n);
void dealloc_page(unsigned char* phys_addr);

void *memset(void *buf, char c, unsigned long n);

#endif
