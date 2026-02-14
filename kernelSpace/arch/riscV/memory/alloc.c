#include "../../../../include/kernelSpace/arch/riscV/memory/alloc.h"

unsigned char* alloc_pages(uint64_t n) {
	if(n <= 0) PANIC("Error alloc page. n <= 0");

	uint64_t sz = n;
	
	int i = 0, endAddr = ((uint64_t)_free_ram_end - (uint64_t)_free_ram_start) / PAGE_SIZE;

	for(; i < endAddr && sz > 0; ++i) {
		if(!buffFreeRam[i]) --sz;
		else sz = n;
	}

	if(sz > 0) PANIC("Error alloc page. Address is full");
	
	int start = i - n;
	if(start < 0) PANIC("Error alloc page. Go beyond");
	
	int end = i;
	for(; start < end; ++start) buffFreeRam[start] = true;

	uint64_t addr = (((uint64_t)_free_ram_start) + ((i - n) * PAGE_SIZE));

	memset((void*)addr, 0, n * PAGE_SIZE);

	return (unsigned char*)addr;
}

void dealloc_page(unsigned char* phys_addr, uint64_t n) {
	if(n <= 0) PANIC("Error dealloc page. n <= 0");

	if((uint64_t)phys_addr < (uint64_t)_free_ram_start || (uint64_t)phys_addr >= (uint64_t)_free_ram_end) PANIC("Error dealloc page. Address invalid");

	uint64_t addr = ((uint64_t)phys_addr - (uint64_t)_free_ram_start) / PAGE_SIZE;

	if(addr + n > MAX_RAM) PANIC("Error dealloc page. Address is full");
	
	for(int i = 0; i < n; ++i) {
		if(!buffFreeRam[addr + i]) PANIC("Error dealloc page. Double free");
		buffFreeRam[addr + i] = false;
	}
}

