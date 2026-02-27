#ifndef __KDEBUG_H__
#define __KDEBUG_H__

#include "../config.h"
#include <stdarg.h>
#include <stdint.h>
#include "../libsbi/le.h"
#include "../libsbi/dbcn.h"
#include "../drivers/uart/uart.h"

extern void halt(void);

console_ret_t console_printf(const unsigned char* format, ...);
uint8_t console_getchar(void);

#define PANIC(format, ...)											\
	do {													\
		console_printf("PANIC: %s:%d: " format "\r\n", __FILE__, __LINE__, ##__VA_ARGS__);		\
														\
		halt();												\
	} while(0)

#endif
