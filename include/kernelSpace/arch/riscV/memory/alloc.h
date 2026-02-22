#ifndef __ALLOC_H__
#define __ALLOC_H__

#include <stdbool.h>
#include "../../../debug/kdebug.h"
#include "../../../libc/string.h"

#define PAGE_SIZE 4096
#define MAX_RAM 16384 // (64 * 1024 * 1024) / PAGE_SIZE 
#define KERNEL_OFFSET 0xffffffff00000000

extern unsigned char _free_ram_start[];
extern unsigned char _free_ram_end[];

static bool buffFreeRam[MAX_RAM];

unsigned char* alloc_pages(uint64_t n);
void dealloc_page(unsigned char* phys_addr, uint64_t n);

#endif
