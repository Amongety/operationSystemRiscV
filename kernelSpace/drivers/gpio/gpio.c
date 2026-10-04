#include "../../include/kernelSpace/drivers/gpio/gpio.h"

void initGPIO(uint64_t addr, uint8_t mode) {
    volatile uint32_t* SWPORTA_DDR = (uint32_t*)(addr + OFFSET_GPIO_SWPORTA_DDR);

    *SWPORTA_DDR = mode;
}

void setGpio(uint64_t addr, uint32_t value) {
    volatile uint32_t* SWPORTA_DR = (uint32_t*)(addr + OFFSET_GPIO_SWPORTA_DR);

    *SWPORTA_DR = value;
}

uint32_t getGpio(uint64_t addr) {
    volatile uint32_t* EXT_PORTA = (uint32_t*)(addr + OFFSET_GPIO_EXT_PORTA);

    return *EXT_PORTA;
}