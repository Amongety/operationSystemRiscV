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
	uint32_t exceptCode:31;
	uint32_t interrupt:1;
};

struct tf_csr_t {
	struct scause_t cause;
	uint32_t tval;
	uint32_t epc;
	uint32_t status;
	uint32_t atp;
};

struct trap_frame {
	uint32_t ra;
	uint32_t sp;
	uint32_t gp;
	uint32_t tp;
	uint32_t t0;
	uint32_t t1;
	uint32_t t2;
	uint32_t fp;
	uint32_t s1;
	uint32_t a0;
	uint32_t a1;
	uint32_t a2;
	uint32_t a3;
	uint32_t a4;
	uint32_t a5;
	uint32_t a6;
	uint32_t a7;
	uint32_t s2;
	uint32_t s3;
	uint32_t s4;
	uint32_t s5;
	uint32_t s6;
	uint32_t s7;
	uint32_t s8;
	uint32_t s9;
	uint32_t s10;
	uint32_t s11;
	uint32_t t3;
	uint32_t t4;
	uint32_t t5;
	uint32_t t6;
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

struct Process* create_process(unsigned long func);
struct Process schedule(void);
extern void switchProc(struct trap_frame* addrFrame, uint32_t epc, uint32_t atp); 

void sub_rotate(void);

#endif
