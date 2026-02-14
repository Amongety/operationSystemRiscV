#include "../../../../include/kernelSpace/arch/riscV/memory/mmu.h"

void map_page(struct Table* root, unsigned char* vaddr, unsigned char* paddr, uint64_t flags) {
	uint64_t vpn[3] = {((((uint64_t)vaddr) >> 12) & 0x1FF), ((((uint64_t)vaddr) >> 21) & 0x1FF), ((((uint64_t)vaddr) >> 30) & 0x1FF)};
	uint64_t ppn[3] = {((((uint64_t)paddr) >> 12) & 0x1FF), ((((uint64_t)paddr) >> 21) & 0x1FF), ((((uint64_t)paddr) >> 30) & 0x3FFFFFF)};

	struct PageEntry* v = (struct PageEntry*)&root->entry[vpn[2]];

	for(int i = 2; i > 0; --i) {
		if(v->V == 0) {
			uint64_t t = (uint64_t)alloc_pages(1);

			v->V = 1;
			v->PPN0 = t >> 12;
			v->PPN1 = t >> 21;
			v->PPN2 = t >> 30;
		}

		struct Table* tmp = (struct Table*) (((uint64_t)v->PPN2 << 30) | ((uint64_t)v->PPN1 << 21) | ((uint64_t)v->PPN0 << 12));
		v = (struct PageEntry*) &tmp->entry[vpn[i - 1]];
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
	v->PPN0 = ppn[0];
	v->PPN1 = ppn[1];
	v->PPN2 = ppn[2];
	v->Reserved = 0;
	v->PBMT = (flags >> 61) & 0x3;
	v->N = 0;
}

void unmap(struct Table* root) {
	if(root == NULL) return;

	for(int lvl2 = 0; lvl2 < EntryLen; ++lvl2) {
		if(root->entry[lvl2].V == 1 && (root->entry[lvl2].X == 0 && root->entry[lvl2].W == 0 && root->entry[lvl2].R == 0)) {
			uint64_t PAT1 = (uint64_t) (((uint64_t)root->entry[lvl2].PPN2 << 30) | ((uint64_t)root->entry[lvl2].PPN1 << 21) | ((uint64_t)root->entry[lvl2].PPN0 << 12)); // physAddrTable
			struct Table* tb_lvl1 = (struct Table*)PAT1;

			for(int lvl1 = 0; lvl1 < EntryLen; ++lvl1) {
				if(tb_lvl1->entry[lvl1].V == 1 && (tb_lvl1->entry[lvl1].X == 0 && tb_lvl1->entry[lvl1].W == 0 && tb_lvl1->entry[lvl1].R == 0)) {
					uint64_t PAT0 = (uint64_t) (((uint64_t)tb_lvl1->entry[lvl1].PPN2 << 30) | ((uint64_t)tb_lvl1->entry[lvl1].PPN1 << 21) | ((uint64_t)tb_lvl1->entry[lvl1].PPN0 << 12));

					dealloc_page((unsigned char*)PAT0, 1);
				}
			}

			dealloc_page((unsigned char*)PAT1, 1);
		}
	}

	dealloc_page((unsigned char*)root, 1);
}

void id_map_page_range(struct Table* root, unsigned char* start, unsigned char* end, uint64_t flags) {
	uint64_t addr_end = (((uint64_t)end) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

	for(uint64_t i = ((uint64_t)start) & ~(PAGE_SIZE - 1); i < addr_end; i += PAGE_SIZE) {
		map_page(root, (unsigned char*)i, (unsigned char*)i, flags);
	}
}

