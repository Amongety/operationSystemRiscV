#include "../../../../include/kernelSpace/arch/riscV/memory/mmu.h"

void map_page(struct Table* root, unsigned char* vaddr, unsigned char* paddr, unsigned long flags) {
	unsigned long vpn[2] = {((((unsigned long)vaddr) >> 12) & 0x3FF), ((((unsigned long)vaddr) >> 22) & 0x3FF)};
	unsigned long pnn[2] = {((((unsigned long)paddr) >> 12) & 0x3FF), ((((unsigned long)paddr) >> 22) & 0xFFF)};

	struct PageEntry* v = &root->entry[vpn[1]];

	for(int i = 1; i > 0; --i) {
		if(v->V == 0) {
			unsigned long t = (unsigned long)alloc_pages(1);

			v->V = 1;
			v->PNN0 = t >> 12;
			v->PNN1 = t >> 22;
		}

		struct Table* tmp = (struct Table*) ((v->PNN1 << 22) | (v->PNN0 << 12));
		v = &tmp->entry[vpn[i - 1]];
	}
	
	v->V = 1;
	v->R = (flags >> 1) & 0x1;
	v->W = (flags >> 2) & 0x1;
	v->X = (flags >> 3) & 0x1;
	v->U = (flags >> 4) & 0x1;
	v->G = (flags >> 5) & 0x1;
	v->A = (flags >> 6) & 0x1;
	v->D = (flags >> 7) & 0x1;
    	v->RSW = (flags >> 8) & 0x3;
	v->PNN0 = pnn[0];
	v->PNN1 = pnn[1];
}

void unmap_page(struct Table* root) {
}

void id_map_page_range(struct Table* root, unsigned char* start, unsigned char* end, unsigned long flags) {
	unsigned long addr_end = (((unsigned long)end) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

	for(unsigned long i = ((unsigned long)start) & ~(PAGE_SIZE - 1); i < addr_end; i += PAGE_SIZE) {
		map_page(root, (unsigned char*)i, (unsigned char*)i, flags);
	}
}

