#ifndef __PLATFORM_DEVICES_H__
#define __PLATFORM_DEVICES_H__

#define UART_MAX 5

struct Uart {
	uint64_t addr;
	uint64_t size;
};

struct dtbPlatform {
	struct Uart uart[UART_MAX];
};

extern struct dtbPlatform dtbPlt;

#endif
