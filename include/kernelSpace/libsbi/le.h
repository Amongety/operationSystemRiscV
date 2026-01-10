/* Legacy Extensions */

#ifndef __LE_H__
#define __LE_H__

#include "../config.h"
#include "sbi.h"

extern long sbi_console_putchar(int ch);

extern long sbi_console_getchar(void);

#endif
