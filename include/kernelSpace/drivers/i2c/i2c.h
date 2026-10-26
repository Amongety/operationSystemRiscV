#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>

struct I2cReg {
    volatile uint32_t IC_CON;
    volatile uint32_t IC_TAR;
    volatile uint32_t IC_SAR;
    volatile uint32_t IC_DATA_CMD;
    volatile uint32_t IC_SS_SCL_HCNT;
    volatile uint32_t IC_SS_SCL_LCNT;
    volatile uint32_t IC_FS_SCL_HCNT;
    volatile uint32_t IC_FS_SCL_LCNT;
    volatile uint32_t IC_INTR_STAT;
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
    volatile uint32_t IC_STATUS;
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

#endif