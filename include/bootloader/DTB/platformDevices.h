#ifndef __PLATFORM_DEVICES_H__
#define __PLATFORM_DEVICES_H__

#define UART_MAX 5
#define GPIO_MAX 5

struct Uart {
	uint64_t addr;
	uint64_t size;
};

struct SdCard {
	uint64_t addr;
	uint64_t size;
};

struct Gpio {
	uint64_t addr;
	uint64_t size;
};

struct DtbPlatform {
	struct Uart uart[UART_MAX];
	struct SdCard sd;
	struct Gpio gpio[GPIO_MAX];
};

extern struct DtbPlatform dtbPlt;

#endif
