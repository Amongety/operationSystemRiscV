#include "../../include/kernelSpace/process/process_control.h"

struct Process bufProc[PROCESS_MAX];
struct Process* prevProcess = NULL;

struct Process* create_process(unsigned long func) {
	static unsigned int nPID = 0;

	struct Process* newProc = NULL;

	for(int i = 0; i < PROCESS_MAX; ++i) {
		if(bufProc[i].state == UN_USED) {
			newProc = &bufProc[i];
			break;
		}
	}

	if(newProc == NULL) PANIC("Process unit full\n");

	newProc->frame = (struct trap_frame*)alloc_pages(1);
	newProc->processStack = alloc_pages(2);
	newProc->root = (struct Table*)alloc_pages(1);
	newProc->processData = alloc_pages(1);
	newProc->pid = nPID;
	newProc->state = SLEEPING;
	newProc->frame->sp = STACK_ADDR + PAGE_SIZE * 2;
	newProc->frame->fp = STACK_ADDR + PAGE_SIZE * 2;
	newProc->frame->csr_reg.epc = func;
	newProc->frame->gp = func + 0x1000; 	// временно
	newProc->frame->csr_reg.atp = 0x80000000 | (newProc->pid << 22) | (((uint32_t)newProc->root) >> 12);

	++nPID;

	unsigned long phys_addr_stack = ((unsigned long)newProc->processStack) & (~(PAGE_SIZE - 1));

	id_map_page_range(newProc->root, (unsigned char*)_free_ram_start, (unsigned char*)_free_ram_end, 0x6);
	id_map_page_range(newProc->root, (unsigned char*)_text_start, (unsigned char*)_text_end, 0xA);
	id_map_page_range(newProc->root, (unsigned char*)_rodata_start, (unsigned char*)_rodata_end, 0xA);
	id_map_page_range(newProc->root, (unsigned char*)_data_start, (unsigned char*)_data_end, 0x6);
	id_map_page_range(newProc->root, (unsigned char*)_bss_start, (unsigned char*)_bss_end, 0x6);
	id_map_page_range(newProc->root, (unsigned char*)_stack - 128 * 1024, (unsigned char*)_stack, 0x6);
	
	for(int i = 0; i < 17; ++i) map_page(newProc->root, (unsigned char*)(STACK_ADDR + PAGE_SIZE * i), (unsigned char*)(phys_addr_stack + PAGE_SIZE * i), 0x16);
	map_page(newProc->root, (unsigned char*)newProc->frame->gp, (unsigned char*)newProc->processData, 0x16);			// временно
	unsigned long alignFunc = func & (~(PAGE_SIZE - 1));
	map_page(newProc->root, (unsigned char*)(PROC_START_ADDR), (unsigned char*)(alignFunc), 0x1E);
	map_page(newProc->root, (unsigned char*)(PROC_START_ADDR + PAGE_SIZE), (unsigned char*)(alignFunc + PAGE_SIZE), 0x1E);

	return newProc;
}

void delete_process(struct Process* proc) {
	sbi_console_printf("Curent Process deleting %d\n", proc->pid);

	dealloc_page((unsigned char*)proc->frame, 1);
	dealloc_page((unsigned char*)proc->processStack, 2);
	dealloc_page((unsigned char*)proc->processData, 1);
	unmap(proc->root);

	memset((void*)proc, 0, sizeof(struct Process));
}

struct Process schedule() {
	bufProc[0].state = SLEEPING;

	sub_rotate();

	for(int i = 0; i < PROCESS_MAX; ++i) {
		if(bufProc[0].state == SLEEPING) {
			bufProc[0].state = RUNNABLE;
			break;
		}

		sub_rotate();
	}

	sbi_console_printf("Current PID: %d\n", bufProc[0].pid);

	prevProcess = &bufProc[0];

	return bufProc[0];
}

void sub_rotate() {
	struct Process start = bufProc[0];

	for(int i = 0; i < PROCESS_MAX - 1; ++i) {
		bufProc[i] = bufProc[i + 1];
	}

	bufProc[PROCESS_MAX - 1] = start;
}

