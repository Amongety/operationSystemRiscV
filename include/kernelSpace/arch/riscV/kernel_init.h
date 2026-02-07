#ifndef __KERNEL_INIT_H__
#define __KERNEL_INIT_H__

#include "memory/mmu.h"
#include "htrap.h"

extern void kernel_init(void);
extern void init_virtual_memory(uint32_t* root); 

#endif
