#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>

extern struct scause_t read_csr_scause(void);
extern uint32_t read_csr_stval(void);
extern uint32_t read_csr_sepc(void);

#endif
