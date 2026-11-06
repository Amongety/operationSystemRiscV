#ifndef __I2C_H__
#define __I2C_H__

#include "../mmio.h"
#include "../../libc/string.h"

struct IC_CON {
    volatile uint32_t MASTER_MODE:1;
    volatile uint32_t SPEED:2;
    volatile uint32_t IC_10BITADDR_SLAVE:1;
    volatile uint32_t IC_10BITADDR_MASTER:1;
    volatile uint32_t IC_RESTART_EN:1;
    volatile uint32_t IC_SLAVE_DISABLE:1;
    volatile uint32_t RES:25;
};

struct IC_TAR {
    volatile uint32_t IC_TAR:10;
    volatile uint32_t GC_OR_START:1;
    volatile uint32_t SPECIAL:1;
    volatile uint32_t RES:20; 
};

struct IC_DATA_CMD {
    volatile uint32_t DAT:8;
    volatile uint32_t CMD:1;
    volatile uint32_t STOP:1;
    volatile uint32_t RESTART:1;
    volatile uint32_t RES:21;
};

struct IC_STATUS {
    volatile uint32_t ST_ACTIVITY:1;
    volatile uint32_t ST_TFNF:1;
    volatile uint32_t ST_TFE:1;
    volatile uint32_t ST_RFNE:1;
    volatile uint32_t ST_RFF:1;
    volatile uint32_t ST_MST_ACTIVITY:1;
    volatile uint32_t ST_SLV_ACTIVITY:1;
    volatile uint32_t RES:25;
};

struct IC_INTR_STAT {
    volatile uint32_t R_RX_UNDER:1;
    volatile uint32_t R_RX_OVER:1;
    volatile uint32_t R_RX_FULL:1;
    volatile uint32_t R_TX_OVER:1;
    volatile uint32_t R_TX_EMPTY:1;
    volatile uint32_t R_RD_REQ:1;
    volatile uint32_t R_TX_ABRT:1;
    volatile uint32_t R_RX_DONE:1;
    volatile uint32_t R_ACTIVITY:1;
    volatile uint32_t R_STOP_DET:1;
    volatile uint32_t R_START_DET:1;
    volatile uint32_t R_GEN_CALL:1;
    volatile uint32_t RES:20;
};

struct I2cReg {
    volatile struct IC_CON IC_CON;
    volatile struct IC_TAR IC_TAR;
    volatile uint32_t IC_SAR;
    volatile uint32_t RES0;
    volatile struct IC_DATA_CMD IC_DATA_CMD;
    volatile uint32_t IC_SS_SCL_HCNT;
    volatile uint32_t IC_SS_SCL_LCNT;
    volatile uint32_t IC_FS_SCL_HCNT;
    volatile uint32_t IC_FS_SCL_LCNT;
    volatile uint32_t RES1[2];
    volatile struct IC_INTR_STAT IC_INTR_STAT;
    volatile uint32_t IC_INTR_MASK;
    volatile uint32_t IC_RAW_INTR_STAT;
    volatile uint32_t IC_RX_TL;
    volatile uint32_t IC_TX_TL;
    volatile uint32_t IC_CLR_INTR;
    volatile uint32_t IC_CLR_RX_UNDER;
    volatile uint32_t IC_CLR_RX_OVER;
    volatile uint32_t IC_CLR_TX_OVER;
    volatile uint32_t IC_CLR_RD_REQ;
    volatile uint32_t IC_CLR_TX_ABRT;
    volatile uint32_t IC_CLR_RX_DONE;
    volatile uint32_t IC_CLR_ACTIVITY;
    volatile uint32_t IC_CLR_STOP_DET;
    volatile uint32_t IC_CLR_START_DET;
    volatile uint32_t IC_CLR_GEN_CALL;
    volatile uint32_t IC_ENABLE;
    volatile struct IC_STATUS IC_STATUS;
    volatile uint32_t IC_TXFLR;
    volatile uint32_t IC_RXFLR;
    volatile uint32_t IC_SDA_HOLD;
    volatile uint32_t IC_TX_ABRT_SOURCE;
    volatile uint32_t IC_SLV_DATA_NACK_ONLY;
    volatile uint32_t IC_DMA_CR;
    volatile uint32_t IC_DMA_TDLR;
    volatile uint32_t IC_DMA_RDLR;
    volatile uint32_t IC_SDA_SETUP;
    volatile uint32_t IC_ACK_GENERAL_CALL;
    volatile uint32_t IC_ENABLE_STATUS;
    volatile uint32_t IC_FS_SPKLEN;
    volatile uint32_t IC_HS_SPKLEN;
};

enum I2Cmode {
    MASTER=0,
    SLAVE
};

void init_i2c(uint64_t addr, enum I2Cmode mode, uint64_t addrSlave);
void writeI2c(uint64_t addr, uint8_t* data, uint64_t lenData);
void readI2c(uint64_t addr, uint8_t* data, uint64_t lenData);
void writeReadI2c(uint64_t addr, uint8_t* reg, uint64_t regLen, uint8_t* rData, uint64_t rLen);

#endif