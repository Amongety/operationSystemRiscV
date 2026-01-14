#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>

struct scause_t {
	uint32_t wlrl:31;
	uint32_t interrupt:1;
};

extern struct scause_t read_csr_scause(void);
extern uint32_t read_csr_stval(void);
extern uint32_t read_csr_sepc(void);

#endif
