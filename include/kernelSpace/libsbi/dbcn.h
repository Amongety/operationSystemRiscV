/* Debug Console Extension */

#ifndef __DBCN_H__
#define __DBCN_H__

#include "../config.h"
#include "sbi.h"

extern struct sbiret sbi_debug_console_write(unsigned long num_bytes, unsigned long *base_addr);

extern struct sbiret sbi_debug_console_read(unsigned long num_bytes, unsigned long *base_addr);

extern struct sbiret sbi_debug_console_write_byte(unsigned char byte);

#endif
