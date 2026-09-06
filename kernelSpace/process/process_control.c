#include "../../include/kernelSpace/process/process_control.h"

struct Process bufProc[PROCESS_MAX] = {0};
struct Process* prevProcess = NULL;

struct Process* create_process(const char* path) {
	static unsigned int nPID = 1;
	long fd = openMinix3(path, 0755);

	if(fd == -1) PANIC("Error open");

	struct Process* newProc = NULL;

	for(int i = 0; i < PROCESS_MAX; ++i) {
		if(bufProc[i].state == UN_USED) {
			newProc = &bufProc[i];
			break;
		}
	}

	if(newProc == NULL) PANIC("Process unit full");

    uint32_t size = getInodeSize(fd);
    int n = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    char *buff = alloc_pages(n);

    int r = readInodeMinix3(fd, (void*)buff, size, 0);
    if(r != size) PANIC("Error read");

	newProc->frame = (struct TrapFrame*)alloc_pages(1);
	newProc->processStack = alloc_pages(35);
	newProc->root = (struct Table*)alloc_pages(1);
	newProc->pid = nPID++;
	newProc->state = SLEEPING;

	struct Elf64ProgramHeader* ph;
	int phnum;
	uint64_t programPages = 0, pc = 0;

	if((phnum = loadElf64(buff, size, &ph, &pc)) != -1) {
		for(int i = 0; i < phnum; ++i) {
			if(ph[i].type != PH_TYPE_LOAD) continue;
			if(ph[i].memsz == 0) continue;
			programPages += ph[i].memsz;
		}

		programPages = (programPages + PAGE_SIZE * 2) / PAGE_SIZE;
		
		newProc->program = alloc_pages(programPages);
		newProc->programSize = programPages;

		unsigned char* programMem = newProc->program;
		
		for(int i = 0; i < phnum; ++i) {
			if(ph[i].type != PH_TYPE_LOAD || ph[i].memsz == 0) continue;
			
			memcpy((void*)programMem, (void*)(buff + ph[i].offset), ph[i].filesz);
			
			uint32_t fields = 0xC0 | 0x10;

			if((ph[i].flags & PH_EXECUTE) != 0) fields |= 0x8;
			if((ph[i].flags & PH_WRITE) != 0) fields |= 0x4;
			if((ph[i].flags & PH_READ) != 0) fields |= 0x2;

			uint32_t pages = (ph[i].memsz + PAGE_SIZE - 1) / PAGE_SIZE;

			for(int p = 0; p < pages; ++p) {
				map_page(newProc->root, (unsigned char*)(ph[i].vaddr + p * PAGE_SIZE), (unsigned char*)(programMem + p * PAGE_SIZE), fields);
			}

			programMem += pages * PAGE_SIZE;
    	}
	}
	
	uint64_t phys_addr_stack = ((uint64_t)newProc->processStack) & (~(PAGE_SIZE - 1));
	for(int i = 0; i < 35; ++i) map_page(newProc->root, (unsigned char*)(STACK_ADDR + PAGE_SIZE * i), (unsigned char*)(phys_addr_stack + PAGE_SIZE * i), 0x16 | 0xC0);
	newProc->frame->csr_reg.epc = pc;
	newProc->frame->sp = STACK_ADDR + 35 * PAGE_SIZE - 0x1000;
	newProc->frame->csr_reg.atp = (uint64_t)(0x8000000000000000 | ((uint64_t)newProc->pid << 44) | (((uint64_t)newProc->root) >> 12));
	
	/* временно мапятся. Пока думаю что сделать с переключением контекста и страницами */
	id_map_page_range(newProc->root, (unsigned char*)_free_ram_start, (unsigned char*)_free_ram_end, 0x6 | 0xC0);
	id_map_page_range(newProc->root, (unsigned char*)_text_start, (unsigned char*)_text_end, 0xA | 0xC0);
	id_map_page_range(newProc->root, (unsigned char*)_rodata_start, (unsigned char*)_rodata_end, 0xA | 0xC0);
	id_map_page_range(newProc->root, (unsigned char*)_data_start, (unsigned char*)_data_end, 0x6 | 0xC0);
	id_map_page_range(newProc->root, (unsigned char*)_bss_start, (unsigned char*)_bss_end, 0x6 | 0xC0);
	id_map_page_range(newProc->root, (unsigned char*)_stack - 128 * 1024, (unsigned char*)_stack, 0x6 | 0xC0);
	map_page(newProc->root, (unsigned char*)globalDTB.uart[0].addr, (unsigned char*)globalDTB.uart[0].addr, 0x6 | 0xC0);
	map_page(newProc->root, (unsigned char*)globalDTB.sd.addr, (unsigned char*)globalDTB.sd.addr, 0x6 | 0xC0);

	dealloc_page(buff, n);

	return newProc;
}

void delete_process(struct Process* proc) {
	console_printf("Current Process deleting %d\r\n", proc->pid);

	dealloc_page((unsigned char*)proc->frame, 1);
	dealloc_page((unsigned char*)proc->processStack, 35);
	dealloc_page((unsigned char*)proc->program, proc->programSize);
	unmap(proc->root);

	memset((void*)proc, 0, sizeof(struct Process));
}

struct Process* schedule() {
	bufProc[0].state = SLEEPING;

	sub_rotate();

	for(int i = 0; i < PROCESS_MAX; ++i) {
		if(bufProc[0].state == SLEEPING) {
			bufProc[0].state = RUNNABLE;
			break;
		}

		sub_rotate();
	}

	console_printf("Current PID: %d\r\n", bufProc[0].pid);

	prevProcess = &(bufProc[0]);

	return &(bufProc[0]);
}

void sub_rotate() {
	struct Process start = bufProc[0];

	for(int i = 0; i < PROCESS_MAX - 1; ++i) {
		bufProc[i] = bufProc[i + 1];
	}

	bufProc[PROCESS_MAX - 1] = start;
}

