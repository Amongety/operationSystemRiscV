#ifndef __ALLOC_H__
#define __ALLOC_H__

#include <stdbool.h>
#include "../../../libsbi/sci.h"
#include "../../../libc/string.h"

#define PAGE_SIZE 4096
#define MAX_RAM 16384 // (64 * 1024 * 1024) / PAGE_SIZE 

extern unsigned char _free_ram_start[];
extern unsigned char _free_ram_end[];

static bool buffFreeRam[MAX_RAM];

unsigned char* alloc_pages(uint32_t n);
void dealloc_page(unsigned char* phys_addr, uint32_t n);

#endif
