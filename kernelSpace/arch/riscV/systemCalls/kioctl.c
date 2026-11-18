#include "../../include/kernelSpace/arch/riscV/systemCalls/kioctl.h"

void do_ioctl(uint64_t device, uint64_t op, void* arg) {
    switch(device) {
        case 0 ... 3:                                                               // UART
        {
            if(op == 0) init_uart(&globalDTB.uart[device], *(uint64_t*)arg);
            else if(op == 1) writeUart(&globalDTB.uart[device], *(uint8_t*)arg);
            else if(op == 2) *(uint8_t*)arg = readUart(&globalDTB.uart[device]);

            break;
        }
        case 4:                                                                     // SD CARD
        {
            uint64_t* type = (uint64_t*)arg, *index = ((uint64_t*)arg) + 1, *size = ((uint64_t*)arg) + 2;
            char* buf = (char*)(((uint64_t*)arg) + 3);

            if(op == 0) init_sdmmc(&globalDTB.sd);
            else if(op == 1) writeSDMMC(&globalDTB.sd, *type, *index, (void*)buf, *size);
            else if(op == 2) readSDMMC(&globalDTB.sd, *type, *index, (void*)buf, *size);

            break;
        }
        case 5 ... 9:                                                               // GPIO
        {
            uint64_t* pin = (uint64_t*)arg;
            uint64_t* modeOrValue = ((uint64_t*)arg) + 1;

            if(op == 0) init_GPIO(&globalDTB.gpio[device - 5], *pin, *modeOrValue);
            else if(op == 1) setGpio(&globalDTB.gpio[device - 5], *pin, *modeOrValue);
            else if(op == 2) *modeOrValue = getGpio(&globalDTB.gpio[device - 5], *pin);
            
            break;
        }
        case 10 ... 14:                                                             // I2C
        {
            uint64_t* modeOrBufOrReg = ((uint64_t*)arg);
            uint64_t* addrOrLenDataOrRData = ((uint64_t*)arg) + 1;
            uint64_t* rLen = ((uint64_t*)arg) + 2;

            if(op == 0) init_i2c(&globalDTB.i2c[device - 10], *modeOrBufOrReg, *addrOrLenDataOrRData);
            else if(op == 1) writeI2c(&globalDTB.i2c[device - 10], (uint8_t*)*modeOrBufOrReg, *addrOrLenDataOrRData);
            else if(op == 2) readI2c(&globalDTB.i2c[device - 10], (uint8_t*)*modeOrBufOrReg, *addrOrLenDataOrRData);
            else if(op == 3) readRegI2c(&globalDTB.i2c[device - 10], *modeOrBufOrReg, (uint8_t*)*addrOrLenDataOrRData, *rLen);

            break;
        }
    }
}