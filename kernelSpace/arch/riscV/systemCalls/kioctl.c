#include "../../include/kernelSpace/arch/riscV/systemCalls/kioctl.h"

void do_ioctl(uint64_t device, uint64_t op, void* arg) {
    switch(device) {
        case 2 ... 6:
            {
            uint64_t* pin = (uint64_t*)arg;
            uint64_t* mode = ((uint64_t*)arg) + 1;
            uint64_t* value = ((uint64_t*)arg) + 2;

            if(op == 0) initGPIO(globalDTB.gpio[device - 2].addr, *pin, *mode);
            else if(op == 1) setGpio(globalDTB.gpio[device - 2].addr, *pin, *value);
            else if(op == 2) *value = getGpio(globalDTB.gpio[device - 2].addr, *pin);
            
            break;
            }
    }
}