#ifndef __HTRAP_H__
#define __HTRAP_H__

#include "common.h"
#include "../../libsbi/sci.h"

struct scause_t {
	uint32_t exceptCode:31;
	uint32_t interrupt:1;
};

struct tf_csr_t {
	struct scause_t cause;
	uint32_t tval;
	uint32_t epc;
	uint32_t status;
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

uint32_t handler_trap(struct trap_frame* t);

#endif
