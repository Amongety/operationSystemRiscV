#ifndef __KERNEL_INIT_H__
#define __KERNEL_INIT_H__

#include "htrap.h"

extern void kernel_init(void);
extern void init_virtual_memory(uint64_t* root); 

#endif
