/* sbi console interface */

#ifndef __SCI_H__
#define __SCI_H__

#include "../config.h"
#include <stdarg.h>
#include <stdint.h>
#include "le.h"
#include "dbcn.h"

extern void halt(void);

sbi_ret_t sbi_console_printf(const unsigned char* format, ...);

#define PANIC(format, ...)											\
	do {													\
		sbi_console_printf("PANIC: %s:%d: " format "\n", __FILE__, __LINE__, ##__VA_ARGS__);		\
														\
		halt();												\
	} while(0)

#endif
