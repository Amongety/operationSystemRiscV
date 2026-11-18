#include "../../../include/kernelSpace/drivers/gpio/gpio.h"

void init_GPIO(struct Gpio *gpioInfo, uint64_t pin, uint8_t mode) {
    volatile uint32_t* SWPORTA_DDR = (uint32_t*)(gpioInfo->addr + OFFSET_GPIO_SWPORTA_DDR);
    volatile uint32_t* INT_POLARITY = (uint32_t*)(gpioInfo->addr + OFFSET_GPIO_INT_POLARITY);

    if(mode) *SWPORTA_DDR |= (1 << pin);
    else *SWPORTA_DDR &= (~(1 << pin));
}

void setGpio(struct Gpio *gpioInfo, uint64_t pin, uint32_t value) {
    volatile uint32_t* SWPORTA_DR = (uint32_t*)(gpioInfo->addr + OFFSET_GPIO_SWPORTA_DR);

    if(value) *SWPORTA_DR |= (1 << pin);
    else *SWPORTA_DR &= (~(1 << pin));
}

uint32_t getGpio(struct Gpio *gpioInfo, uint64_t pin) {
    volatile uint32_t* EXT_PORTA = (uint32_t*)(gpioInfo->addr + OFFSET_GPIO_EXT_PORTA);

    return (*EXT_PORTA >> pin) & 0x1;
}