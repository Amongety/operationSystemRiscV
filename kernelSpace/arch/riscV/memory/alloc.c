#include "../../../../include/kernelSpace/arch/riscV/memory/alloc.h"

unsigned char* alloc_pages(uint32_t n) {
	if(n <= 0) PANIC("Error alloc page. n <= 0");

	static unsigned char* next_phys_addr = _free_ram_start;
	unsigned char* phys_addr = next_phys_addr;

	next_phys_addr += PAGE_SIZE * n;

	if(next_phys_addr > _free_ram_end) PANIC("Error alloc page. Address is full");

	memset((void *) phys_addr, 0, n * PAGE_SIZE);

	return phys_addr;
}

void dealloc_page(unsigned char* phys_addr) {
}

