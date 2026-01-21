#ifndef __KERNEL_INIT_H__
#define __KERNEL_INIT_H__

#include "memory/mmu.h"
#include "htrap.h"

extern void kernel_init(uint32_t* root);

#endif
