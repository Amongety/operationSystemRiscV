#ifndef __GPIO_H__
#define __GPIO_H__

#include "../mmio.h"
#include <stdint.h>

#define GPIO_INPUT 0
#define GPIO_OUTPUT 1

#define OFFSET_GPIO_SWPORTA_DR 0x0
#define OFFSET_GPIO_SWPORTA_DDR 0x4
#define OFFSET_GPIO_INTEN 0x30
#define OFFSET_GPIO_INTMASK 0x34
#define OFFSET_GPIO_INTTYPE_LEVEL 0x38
#define OFFSET_GPIO_INT_POLARITY 0x3c
#define OFFSET_GPIO_INTSTATUS 0x40
#define OFFSET_GPIO_RAW_INTSTATUS 0x44
#define OFFSET_GPIO_DEBOUNCE  0x48
#define OFFSET_GPIO_PORTA_EOI 0x4c
#define OFFSET_GPIO_EXT_PORTA 0x50
#define OFFSET_GPIO_LS_SYNC 0x60

void initGPIO(uint64_t addr, uint8_t mode);
void setGpio(uint64_t addr, uint32_t value);
uint32_t getGpio(uint64_t addr);

#endif