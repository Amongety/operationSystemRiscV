#ifndef __SYS_IOCTL_H__
#define __SYS_IOCTL_H__

#include <stdint.h>

#define SYSCALL_IOCTL                   0x1d

enum DeviceType {
    DEVICE_UART=0,
    DEVICED_SDMMC,
    DEVICE_GPIO0,
    DEVICE_GPIO1,
    DEVICE_GPIO2,
    DEVICE_GPIO3,
    DEVICE_GPIO4,
};

#define GPIO_INPUT 0
#define GPIO_OUTPUT 1

#define GPIO_SET_MODE 0
#define GPIO_WRITE 1
#define GPIO_READ 2

struct GpioArg {
    uint64_t pin;
    uint64_t mode;
    uint64_t value;
};

void ioctl(uint64_t device, uint64_t op, void* arg);

#endif