/* Timer Extension */

#ifndef __TE_H__
#define __TE_H__

#include "sbi.h"

// 1s: stime_value = 10000000
extern struct sbiret sbi_debug_set_timer(uint64_t stime_value);

#endif
