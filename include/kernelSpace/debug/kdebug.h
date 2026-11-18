#ifndef __KDEBUG_H__
#define __KDEBUG_H__

#include <stdarg.h>
#include "../config.h"
#include "../libsbi/le.h"
#include "../libsbi/dbcn.h"
#include "../drivers/uart/uart.h"

extern void halt(void);

console_ret_t console_printf(struct Uart *uartInfo, const unsigned char* format, ...);
uint8_t console_getchar(struct Uart *uartInfo);

#define PANIC(format, ...)											\
	do {													\
		console_printf(&globalDTB.uart[0], "PANIC: %s:%d: " format "\r\n", __FILE__, __LINE__, ##__VA_ARGS__);		\
														\
		halt();												\
	} while(0)

#endif
