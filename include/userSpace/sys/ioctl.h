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
    
    DEVICE_SDMMC,
    
    DEVICE_GPIO0,
    DEVICE_GPIO1,
    DEVICE_GPIO2,
    DEVICE_GPIO3,
    DEVICE_GPIO4,
    
    DEVICE_I2C0,
    DEVICE_I2C1,
    DEVICE_I2C2,
    DEVICE_I2C3,
    DEVICE_I2C4,
};

#define UART_SET_MODE 0
#define UART_WRITE 1
#define UART_READ 2

enum UARTmode {
	queryMode = 0,
	interruptMode,
	DMAMode
};

struct GpioArg {
    union {uint64_t pin;};
    union {uint64_t mode; uint64_t value;};
};

#define GPIO_SET_MODE UART_SET_MODE
#define GPIO_WRITE UART_WRITE
#define GPIO_READ UART_READ

enum GPIOmode {
    GPIO_INPUT=0,
    GPIO_OUTPUT
};

struct SDMMCArg {
    uint64_t type;
    uint64_t index;
    uint64_t size;
    char *buf;
};

#define SD_CARD_SET_MODE GPIO_SET_MODE
#define SD_CARD_WRITE GPIO_WRITE
#define SD_CARD_READ GPIO_READ

enum SdCardMode {
    NONDMA=0,
    SDMA,
    ADMA
};

struct I2cArg {
    union {uint64_t mode; void *buf; uint64_t reg;}; 
    union {uint64_t addr; uint64_t lenData; void *rData;};
    union {uint64_t rLen;};
};

#define I2C_SET_MODE GPIO_SET_MODE
#define I2C_WRITE GPIO_WRITE
#define I2C_READ GPIO_READ
#define I2C_READ_REG 3

enum I2cMode {
    MASTER=0,
    SLAVE,
};

void ioctl(uint64_t device, uint64_t op, void* arg);

#endif