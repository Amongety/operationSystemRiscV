#include "../../include/kernelSpace/drivers/i2c/i2c.h"

void init_i2c(uint64_t addr, enum I2Cmode mode, uint64_t addrSlave) {
    volatile struct I2cReg *i2c = (struct I2cReg*)addr;

    i2c->IC_ENABLE = 0;

    switch(mode) {
        case MASTER:
            i2c->IC_CON.IC_SLAVE_DISABLE = 1;
            i2c->IC_CON.MASTER_MODE = 1;
            break;
        case SLAVE:
            i2c->IC_CON.IC_SLAVE_DISABLE = 0;
            i2c->IC_CON.MASTER_MODE = 0;
            break;
    };
    i2c->IC_CON.IC_RESTART_EN = 1;
    i2c->IC_CON.IC_10BITADDR_MASTER = 0;
    i2c->IC_CON.IC_10BITADDR_SLAVE = 0;
    i2c->IC_CON.SPEED = 1;

    i2c->IC_TAR.IC_TAR = addrSlave;

    switch(25) {
        case 25:
            i2c->IC_SS_SCL_HCNT = 115;
            i2c->IC_SS_SCL_LCNT = 135;
            i2c->IC_FS_SCL_HCNT = 21;
            i2c->IC_FS_SCL_LCNT = 42;
            i2c->IC_SDA_HOLD = 1;
            i2c->IC_SDA_SETUP = 6;
            i2c->IC_FS_SPKLEN = 2;
            break;
        case 100:
            i2c->IC_SS_SCL_HCNT = 460;
            i2c->IC_SS_SCL_LCNT  = 540;
            i2c->IC_FS_SCL_HCNT = 90;
            i2c->IC_FS_SCL_LCNT = 160;
            i2c->IC_SDA_HOLD = 1;
            i2c->IC_SDA_SETUP = 25;
            i2c->IC_FS_SPKLEN = 5;
            break;
    }

    i2c->IC_INTR_MASK = 0;

    i2c->IC_RX_TL = 0;
    i2c->IC_TX_TL = 0;

    // i2c->IC_ENABLE = 1;

    console_printf(globalDTB.uart[0].addr, "IC_CON = %x\r\nIC_SLAVE_DISABLE = %d\r\nIC_TAR = %x\r\nIC_SS_SCL_HCNT = %d\r\nIC_SS_SCL_LCNT = %d\r\nIC_ENABLE = %x\r\nIC_STATUS = %x\r\nIC_ENABLE_STATUS = %x\r\n", *(uint32_t*)&i2c->IC_CON, i2c->IC_CON.IC_SLAVE_DISABLE, (uint32_t)i2c->IC_TAR.IC_TAR, i2c->IC_SS_SCL_HCNT, i2c->IC_SS_SCL_LCNT, i2c->IC_ENABLE, i2c->IC_STATUS, i2c->IC_ENABLE_STATUS);

    /*for(int i = 0; i < 100000; ++i) {
        if(i2c->IC_ENABLE_STATUS & 1) {
            console_printf(globalDTB.uart[0].addr, "Sucsesfull init i2c\n\r");
            break;
        }
    }

    if(!(i2c->IC_ENABLE_STATUS & 1)) console_printf(globalDTB.uart[0].addr, "ERROR???\n\r");*/
}

void writeI2c(uint64_t addr, uint8_t* data, uint64_t lenData) {
    volatile struct I2cReg *i2c = (struct I2cReg*)addr;

    i2c->IC_ENABLE = 1;

    for(int i = 0; i < lenData - 1; ++i) {
        // memset((void*)&i2c->IC_DATA_CMD, 0, sizeof(struct IC_DATA_CMD));
        i2c->IC_DATA_CMD.DAT = data[i];
        
        while(i2c->IC_INTR_STAT.R_TX_EMPTY != 0) {};
        console_printf(globalDTB.uart[0].addr, "Int0\r\n");
    }

    // memset((void*)&i2c->IC_DATA_CMD, 0, sizeof(struct IC_DATA_CMD));
    i2c->IC_DATA_CMD.DAT = data[lenData - 1];
    i2c->IC_DATA_CMD.STOP = 1;

    while(i2c->IC_INTR_STAT.R_TX_EMPTY != 0) {};
    console_printf(globalDTB.uart[0].addr, "Int1\r\n");

    while(i2c->IC_STATUS.ST_MST_ACTIVITY != 0) {};
    console_printf(globalDTB.uart[0].addr, "Int2\r\n");

    i2c->IC_ENABLE = 0;

    console_printf(globalDTB.uart[0].addr, "Done\r\n");
}

void readI2c(uint64_t addr, uint8_t* data) {
    volatile struct I2cReg *i2c = (struct I2cReg*)addr;

    i2c->IC_ENABLE = 1;



    i2c->IC_ENABLE = 0;
}
