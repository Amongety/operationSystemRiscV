#ifndef __SYS_IOCTL_H__
#define __SYS_IOCTL_H__

#include <stdint.h>
#include <stdarg.h>

#define SYSCALL_IOCTL                   0x1d

enum DeviceType {
    DEVICE_UART0=0,
    DEVICE_UART1,
    DEVICE_UART2,
    DEVICE_UART3,
    DEVICED_SDMMC,
    DEVICE_GPIO0,
    DEVICE_GPIO1,
    DEVICE_GPIO2,
    DEVICE_GPIO3,
    DEVICE_GPIO4,
};

enum UARTmode {
	queryMode = 0,
	interruptMode,
	DMAMode
};

enum SdCardMode {
	NONDMA=0,
	SDMA,
	ADMA
};

enum GPIOmode {
    GPIO_INPUT=0,
    GPIO_OUTPUT
};

#define GPIO_SET_MODE 0
#define GPIO_WRITE 1
#define GPIO_READ 2

#define UART_SET_MODE GPIO_SET_MODE
#define UART_WRITE GPIO_WRITE
#define UART_READ GPIO_READ

#define SD_CARD_SET_MODE GPIO_SET_MODE
#define SD_CARD_WRITE GPIO_WRITE
#define SD_CARD_READ GPIO_READ

struct GpioArg {
    uint64_t pin;
    uint64_t mode;
    uint64_t value;
};

struct SDMMCArg {
    uint64_t type;
    uint64_t index;
    uint64_t size;
    char *buf;
};

void ioctl(uint64_t device, uint64_t op, void* arg);

#endif