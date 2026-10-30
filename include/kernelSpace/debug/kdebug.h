#ifndef __KDEBUG_H__
#define __KDEBUG_H__

#include "../config.h"
#include <stdarg.h>
#include <stdint.h>
#include "../libsbi/le.h"
#include "../libsbi/dbcn.h"
#include "../drivers/uart/uart.h"
#include "../drivers/mmio.h"
#include "../bootloader/DTB/platformDevices.h"

extern struct DtbPlatform globalDTB;
extern void halt(void);

console_ret_t console_printf(uint64_t addr, const unsigned char* format, ...);
uint8_t console_getchar(void);

#define PANIC(format, ...)											\
	do {													\
		console_printf(globalDTB.uart[0].addr, "PANIC: %s:%d: " format "\r\n", __FILE__, __LINE__, ##__VA_ARGS__);		\
														\
		halt();												\
	} while(0)

#endif
