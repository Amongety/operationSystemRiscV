#ifndef __PROCESS_CONTROL_H__
#define __PROCESS_CONTROL_H__

#include <stddef.h>
#include "../arch/riscV/memory/mmu.h"

#define PROCESS_MAX 8

#define STACK_ADDR 0xf0000000
#define PROC_START_ADDR 0x20000000

extern unsigned char _data_start[];
extern unsigned char _data_end[];
extern unsigned char _bss_start[];
extern unsigned char _bss_end[];
extern unsigned char _stack[];
extern unsigned char _text_start[];
extern unsigned char _text_end[];
extern unsigned char _rodata_start[];
extern unsigned char _rodata_end[];

struct scause_t {
	uint64_t exceptCode:63;
	uint64_t interrupt:1;
};

struct tf_csr_t {
	struct scause_t cause;
	uint64_t tval;
	uint64_t epc;
	uint64_t status;
	uint64_t atp;
};

struct trap_frame {
	uint64_t ra;
	uint64_t sp;
	uint64_t gp;
	uint64_t tp;
	uint64_t t0;
	uint64_t t1;
	uint64_t t2;
	uint64_t fp;
	uint64_t s1;
	uint64_t a0;
	uint64_t a1;
	uint64_t a2;
	uint64_t a3;
	uint64_t a4;
	uint64_t a5;
	uint64_t a6;
	uint64_t a7;
	uint64_t s2;
	uint64_t s3;
	uint64_t s4;
	uint64_t s5;
	uint64_t s6;
	uint64_t s7;
	uint64_t s8;
	uint64_t s9;
	uint64_t s10;
	uint64_t s11;
	uint64_t t3;
	uint64_t t4;
	uint64_t t5;
	uint64_t t6;
	struct tf_csr_t csr_reg;
};

enum ProcessState {
	UN_USED = 0,
	RUNNABLE,
	SLEEPING,
	WAITING,
	EXITED,
};

struct Process {
	struct trap_frame* frame;
	unsigned char* processStack;
	unsigned int pid;
	struct Table* root;
	enum ProcessState state;
	unsigned char *processData;
};

struct Process* create_process(uint64_t func);
void delete_process(struct Process* proc); 
struct Process schedule(void);
extern void switchProc(struct trap_frame* addrFrame, uint64_t epc, uint64_t atp); 

void sub_rotate(void);

#endif
