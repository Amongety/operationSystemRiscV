#ifndef __MMU_H__
#define __MMU_H__

#include "alloc.h"
#include <stddef.h>

#define EntryLen 1024

struct PageEntry {
	unsigned long V:1;
	unsigned long R:1;
	unsigned long W:1;
	unsigned long X:1;
	unsigned long U:1;
	unsigned long G:1;
	unsigned long A:1;
	unsigned long D:1;
	unsigned long RSW:2;
	unsigned long PPN0:10;
	unsigned long PPN1:12;
};

struct Table {
	struct PageEntry entry[EntryLen];
};

void map_page(struct Table* root, unsigned char* vaddr, unsigned char* paddr, uint32_t flags);
void unmap(struct Table* root);
void id_map_page_range(struct Table* root, unsigned char* start, unsigned char* end, uint32_t flags);

#endif
