/* Legacy Extensions */

#ifndef __LE_H__
#define __LE_H__

#include "../config.h"
#include "sbi.h"

// 1s: stime_value = 10000000
extern long sbi_set_timer(uint64_t stime_value);

extern long sbi_console_putchar(int ch);

extern long sbi_console_getchar(void);

#endif
