#ifndef __KIOCTL_H__
#define __KIOCTL_H__

#include "../drivers/gpio/gpio.h"
#include "../drivers/sdmmc/sdmmc.h"
#include "../drivers/uart/uart.h"

void do_ioctl(uint64_t device, uint64_t op, void* arg);

#endif