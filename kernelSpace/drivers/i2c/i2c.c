#include "../../../include/kernelSpace/drivers/i2c/i2c.h"

void init_i2c(struct I2C *i2cInfo, enum I2Cmode mode, uint64_t addrSlave) {
    volatile struct I2cReg *i2c = (struct I2cReg*)i2cInfo->addr;

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

    i2c->IC_INTR_MASK = 0b111111111111;

    i2c->IC_RX_TL = 0;
    i2c->IC_TX_TL = 0;
}

static void sendCommandI2c(uint64_t addr, struct IC_DATA_CMD cmd) {
    volatile struct I2cReg *i2c = (struct I2cReg*)addr;
    
    i2c->IC_DATA_CMD = cmd;
    while(i2c->IC_INTR_STAT.R_TX_EMPTY != 1) {};
}

void writeI2c(struct I2C *i2cInfo, uint8_t* data, uint64_t lenData) {
    volatile struct I2cReg *i2c = (struct I2cReg*)i2cInfo->addr;
    struct IC_DATA_CMD settings = {0};

    i2c->IC_ENABLE = 1;

    for(int i = 0; i < lenData - 1; ++i) {
        settings.DAT = data[i];
        sendCommandI2c(i2cInfo->addr, settings);
    }

    settings.DAT = data[lenData - 1];
    settings.STOP = 1;
    sendCommandI2c(i2cInfo->addr, settings);

    while(i2c->IC_STATUS.ST_MST_ACTIVITY != 0) {};
    i2c->IC_ENABLE = 0;
}

void readI2c(struct I2C *i2cInfo, uint8_t* data, uint64_t lenData) {
    volatile struct I2cReg *i2c = (struct I2cReg*)i2cInfo->addr;
    struct IC_DATA_CMD settings = {.CMD=1};
    uint64_t rxLen = 0, txLen = 0;

    i2c->IC_ENABLE = 1;

    while(rxLen < lenData) {
        while((i2c->IC_STATUS.ST_TFNF != 0) && (rxLen < lenData)) {
            settings.STOP = (rxLen == (lenData - 1)) ? 1:0;
            sendCommandI2c(i2cInfo->addr, settings);

            ++rxLen;
        }

        while(txLen < rxLen) {
            while(i2c->IC_INTR_STAT.R_RX_FULL != 1) {};
            data[txLen] = i2c->IC_DATA_CMD.DAT;
            
            ++txLen;
        }
    }

    while(i2c->IC_STATUS.ST_MST_ACTIVITY != 0) {};
    i2c->IC_ENABLE = 0;
}

void readRegI2c(struct I2C *i2cInfo, uint8_t reg, uint8_t* rData, uint64_t rLen) {
    volatile struct I2cReg *i2c = (struct I2cReg*)i2cInfo->addr;
    struct IC_DATA_CMD settings = {.CMD=0,.DAT=reg,.RESTART=1};
    uint64_t rxLen = 0, txLen = 0;

    i2c->IC_ENABLE = 1;
    
    sendCommandI2c(i2cInfo->addr, settings);

    settings.DAT = 0;
    settings.CMD = 1;
    settings.RESTART = 0;

    while(rxLen < rLen) {
        while((i2c->IC_STATUS.ST_TFNF != 0) && (rxLen < rLen)) {
            settings.STOP = (rxLen == (rLen - 1)) ? 1:0;
            sendCommandI2c(i2cInfo->addr, settings);

            ++rxLen;
        }

        while(txLen < rxLen) {
            while(i2c->IC_INTR_STAT.R_RX_FULL != 1) {};
            rData[txLen] = i2c->IC_DATA_CMD.DAT;
            
            ++txLen;
        }
    }

    while(i2c->IC_STATUS.ST_MST_ACTIVITY != 0) {};
    i2c->IC_ENABLE = 0;
}
