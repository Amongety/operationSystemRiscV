#ifndef __KERNEL_INIT_H__
#define __KERNEL_INIT_H__

#include "htrap.h"

struct DtbPlatform globalDTB;

extern void init_virtual_memory(uint64_t* root); 

#endif
