#include "../../include/kernelSpace/arch/riscV/systemCalls/kioctl.h"

void do_ioctl(uint64_t device, uint64_t op, void* arg) {
    switch(device) {
        case 0 ... 3:                                                               // UART
        {
            if(op == 0) init_uart(globalDTB.uart[device].addr, *(uint64_t*)arg);
            else if(op == 1) uartWrite(globalDTB.uart[device].addr, *(uint8_t*)arg);
            else if(op == 2) *(uint8_t*)arg = uartRead(globalDTB.uart[device].addr);

            break;
        }
        case 4:                                                                     // SD CARD
        {
            uint64_t* type = (uint64_t*)arg, *index = ((uint64_t*)arg) + 1, *size = ((uint64_t*)arg) + 2;
            char* buf = (char*)(((uint64_t*)arg) + 3);

            if(op == 0) init_sdmmc(globalDTB.sd.addr);
            else if(op == 1) writeSDMMC(globalDTB.sd.addr, *type, *index, (void*)buf, *size);
            else if(op == 2) readSDMMC(globalDTB.sd.addr, *type, *index, (void*)buf, *size);

            break;
        }
        case 5 ... 9:                                                               // GPIO
        {
            uint64_t* pin = (uint64_t*)arg;
            uint64_t* mode = ((uint64_t*)arg) + 1;
            uint64_t* value = ((uint64_t*)arg) + 2;

            if(op == 0) initGPIO(globalDTB.gpio[device - 5].addr, *pin, *mode);
            else if(op == 1) setGpio(globalDTB.gpio[device - 5].addr, *pin, *value);
            else if(op == 2) *value = getGpio(globalDTB.gpio[device - 5].addr, *pin);
            
            break;
        }
    }
}