#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>

extern struct scause_t read_csr_scause(void);
extern uint64_t read_csr_stval(void);
extern uint64_t read_csr_sepc(void);
extern uint64_t read_csr_sie(void);

extern void write_csr_sie(uint64_t);

#endif
