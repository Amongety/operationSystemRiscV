#ifndef __MMU_H__
#define __MMU_H__

#include "alloc.h"
#include <stddef.h>

#define EntryLen 512

struct PageEntry {
	uint64_t V:1;
	uint64_t R:1;
	uint64_t W:1;
	uint64_t X:1;
	uint64_t U:1;
	uint64_t G:1;
	uint64_t A:1;
	uint64_t D:1;
	uint64_t RSW:2;
	uint64_t PPN0:9;
	uint64_t PPN1:9;
	uint64_t PPN2:26;
	uint64_t Reserved:7;
	uint64_t PBMT:2;
	uint64_t N:1;
};

struct Table {
	struct PageEntry entry[EntryLen];
};

void map_page(struct Table* root, unsigned char* vaddr, unsigned char* paddr, uint64_t flags);
void unmap(struct Table* root);
void id_map_page_range(struct Table* root, unsigned char* start, unsigned char* end, uint64_t flags);

#endif
