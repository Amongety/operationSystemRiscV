#ifndef __SDMMC_H__
#define __SDMMC_H__

#include "../mmio.h"

enum TransferMode {
	NONDMA=0,
	SDMA,
	ADMA
};

struct SdmmcReg {
	volatile uint32_t SDMA_SADDR;
	volatile uint32_t BLK_SIZE_AND_CNT;
	volatile uint32_t ARGUMENT;
	volatile uint32_t XFER_MODE_AND_CMD;
	volatile uint32_t RESP31_0;
	volatile uint32_t RESP63_32;
	volatile uint32_t RESP95_64;
	volatile uint32_t RESP127_96;
	volatile uint32_t BUF_DATA;
	volatile uint32_t PRESENT_STS;
	volatile uint32_t HOST_CTL1_PWR_BG_WUP;
	volatile uint32_t CLK_CTL_SWRST;
	volatile uint32_t NORM_AND_ERR_INT_STS;
	volatile uint32_t NORM_AND_ERR_INT_STS_EN;
	volatile uint32_t NORM_AND_ERR_INT_SIG_EN;
	volatile uint32_t AUTO_CMD_ERR_AND_HOST_CTL2;
	volatile uint32_t CAPABILITIES1;
	volatile uint32_t CAPABILITIES2;
	volatile uint32_t RESERVED0[2];
	volatile uint32_t FORCE_EVENT_ERR;
	volatile uint32_t ADMA_ERR_STS;
	volatile uint32_t ADMA_SADDR_L;
	volatile uint32_t ADMA_SADDR_H;
	volatile uint32_t PRESENT_VUL_INIT_DS;
	volatile uint32_t PRESENT_VUL_HS_SDR12;
	volatile uint32_t PRESENT_VUL_SDR25_SDR50;
	volatile uint32_t PRESENT_VUL_SDR104_DDR50;
	volatile uint32_t RESERVED1[35];
	volatile uint32_t SLOT_INT_AND_HOST_VER;
	volatile uint32_t RESERVED2[64];
	volatile uint32_t EMMC_CTRL;
	volatile uint32_t EMMC_BOOT_CTL;
	volatile uint32_t CDET_TOUT_CTL;
	volatile uint32_t MBIU_CTRL;
	volatile uint32_t RESERVED3[12];
	volatile uint32_t PHY_TX_RX_DLY;
	volatile uint32_t PHY_DS_DLY;
	volatile uint32_t PHY_DLY_STS;
	volatile uint32_t PHY_CONFIG;
};

#define TIME_WAIT 									1000000

/* BLK_SIZE_AND_CNT */
#define XFER_BLK_SIZE(x) 							((x))
#define SDMA_BUF_BDARY(x) 							((x) << 12)
#define BLK_CNT(x) 									((x) << 16)

/* XFER_MODE_AND_CMD */
#define DMA_ENABLE 									(1U << 0)
#define BLK_CNT_ENABLE 								(1U << 1)
#define AUTO_CMD_ENABLE(x) 							((x) << 2)
#define DAT_XFER_DIR 								(1U << 4)
#define MULTI_BLK_SEL 								(1U << 5)
#define RESP_TYPE 									(1U << 6)
#define RESP_ERR_CHK_ENABLE 						(1U << 7)
#define RESP_INT_DISABLE 							(1U << 8)
#define RESP_TYPE_SEL(x) 							((x) << 16)
#define SUB_CMD_FLAG 								(1U << 18)
#define CMD_CRC_CHK_ENABLE 							(1U << 19)
#define CMD_IDX_CHK_ENABLE 							(1U << 20)
#define DATA_PRESENT_SEL 							(1U << 21)
#define CMD_TYPE(x) 								((x) << 22)
#define CMD_IDX(x) 									((x) << 24)
#define GET_CMD_IDX(x)   							(((x) >> 24) & 0x3F)

/* PRESENT_STS */
#define GET_CMD_INHIBIT(x) 							((x) & 0x1)
#define GET_CMD_INHIBIT_DAT(x) 						(((x) >> 1) & 0x1)
#define GET_DAT_LINE_ACTIVE(x) 						(((x) >> 2) & 0x1)
#define GET_RE_TUNE_REQ(x) 							(((x) >> 3) & 0x1)
#define GET_WR_XFER_ACTIVE(x) 						(((x) >> 8) & 0x1)
#define GET_RD_XFER_ACTIVE(x) 						(((x) >> 9) & 0x1)
#define GET_BUF_WR_ENABLE(x) 						(((x) >> 10) & 0x1)
#define GET_BUF_RD_ENABLE(x) 						(((x) >> 11) & 0x1)
#define GET_CARD_INSERTED(x) 						(((x) >> 16) & 0x1)
#define GET_CARD_STABLE(x) 							(((x) >> 17) & 0x1)
#define GET_CARD_CD_STS(x) 							(((x) >> 18) & 0x1)
#define GET_CARD_WP_STS(x) 							(((x) >> 19) & 0x1)
#define GET_DAT_3_0_STS(x) 							(((x) >> 20) & 0xF)
#define GET_CMD_LINE_STS(x) 						(((x) >> 24) & 0x1)

/* HOST_CTL1_PWR_BG_WUP */
#define LEC_CTL										(1U << 0)
#define DAT_XFER_WIDTH								(1U << 1)
#define HS_ENABLE									(1U << 2)
#define DMA_SEL(x)									((x) << 3)
#define EXT_DAT_WIDTH								(1U << 5)
#define CRAD_DET_TEST								(1U << 6)
#define CARD_DET_SEL								(1U << 7)
#define SD_BUS_PWR									(1U << 8)
#define SD_BUS_VOL_SEL(x)							((x) << 9)
#define STOP_BG_REQ									(1U << 16)
#define CONTINUE_REQ								(1U << 17)
#define READ_WAIT									(1U << 18)
#define INT_BG										(1U << 19)
#define WAKEUP_ON_CARD_INT							(1U << 24)
#define WAKEUP_ON_CARD_INSERT						(1U << 25)
#define WAKEUP_ON_CARD_REMV							(1U << 26)

/* CLK_CTL_SWRST */
#define INT_CLK_EN									(1U << 0)
#define GET_INT_CLK_STABLE(x)						(((x) >> 1) & 0x1)
#define SD_CLK_EN									(1U << 2)
#define PLL_EN										(1U << 3)
#define UP_FREQ_SEL(x)								((x) << 6)
#define FREQ_SEL(x)									((x) << 8)
#define TOUT_CNT(x)									((x) << 16)
#define SW_RST_ALL									(1U << 24)
#define SW_RST_CMD									(1U << 25)
#define SW_RST_DAT									(1U << 26)

/* NORM_AND_ERR_INT_STS */
#define CMD_CMPL									(1U << 0)
#define XFER_CMPL									(1U << 1)
#define BG_EVENT									(1U << 2)
#define DMA_INT										(1U << 3)
#define BUF_WRDY									(1U << 4)
#define BUF_RRDY									(1U << 5)
#define CARD_INSERT_INT								(1U << 6)
#define CARD_REMOV_INT								(1U << 7)
#define CARD_INT									(1U << 8)
#define GET_INT_A(x)								(((x) >> 9) & 0x1)
#define GET_INT_B(x)								(((x) >> 10) & 0x1)
#define GET_INT_C(x)								(((x) >> 11) & 0x1)
#define GET_RE_TUNE_EVENT(x)						(((x) >> 12) & 0x1)
#define GET_CQE_EVENT(x)							(((x) >> 14) & 0x1)
#define GET_ERR_INT(x)								(((x) >> 15) & 0x1)
#define CMD_TOUT_ERR								(1U << 16)
#define CMD_CRC_ERR									(1U << 17)
#define CMD_ENDBIT_ERR								(1U << 18)
#define CMD_IDX_ERR									(1U << 19)
#define DAT_TOUT_ERR								(1U << 20)
#define DAT_CRC_ERR									(1U << 21)
#define DAT_ENDBIT_ERR								(1U << 22)
#define CURR_LIMIT_ERR								(1U << 23)
#define AUTO_CMD_ERR								(1U << 24)
#define ADMA_ERR									(1U << 25)
#define TUNE_ERR									(1U << 26)
#define BOOT_ACK_ERR								(1U << 28)
#define ERR_INT_STS_ERR_MASK            			(0xFFFF8000)

/* NORM_AND_ERR_INT_STS_EN */
#define CMD_CMPL_EN									(1U << 0)
#define XFER_CMPL_EN								(1U << 1)
#define BG_EVENT_EN									(1U << 2)
#define DMA_INT_EN									(1U << 3)
#define BUF_WRDY_EN									(1U << 4)
#define BUF_RRDY_EN									(1U << 5)
#define CARD_INSERT_INT_EN							(1U << 6)
#define CARD_REMOV_INT_EN							(1U << 7)
#define CARD_INT_EN									(1U << 8)
#define INT_A_EN									(1U << 9)
#define INT_B_EN									(1U << 10)
#define INT_C_EN									(1U << 11)
#define RE_TUNE_EVENT_EN							(1U << 12)
#define CQE_EVENT_EN								(1U << 14)
#define ERR_INT_EN									(1U << 15)
#define CMD_TOUT_ERR_EN								(1U << 16)
#define CMD_CRC_ERR_EN								(1U << 17)
#define CMD_ENDBIT_ERR_EN							(1U << 18)
#define CMD_IDX_ERR_EN								(1U << 19)
#define DAT_TOUT_ERR_EN								(1U << 20)
#define DAT_CRC_ERR_EN								(1U << 21)
#define DAT_ENDBIT_ERR_EN							(1U << 22)
#define CURR_LIMIT_ERR_EN							(1U << 23)
#define AUTO_CMD_ERR_EN								(1U << 24)
#define ADMA_ERR_EN									(1U << 25)
#define TUNE_ERR_EN									(1U << 26)
#define BOOT_ACK_ERR_EN								(1U << 28)

/* NORM_AND_ERR_INT_SIG_EN */
#define CMD_CMPL_SIG_EN								(1U << 0)
#define XFER_CMPL_SIG_EN							(1U << 1)
#define BG_EVENT_SIG_EN								(1U << 2)
#define DMA_INT_SIG_EN								(1U << 3)
#define BUF_WRDY_SIG_EN								(1U << 4)
#define BUF_RRDY_SIG_EN								(1U << 5)
#define CARD_INSERT_INT_SIG_EN						(1U << 6)
#define CARD_REMOV_INT_SIG_EN						(1U << 7)
#define CARD_INT_SIG_EN								(1U << 8)
#define INT_A_SIG_EN								(1U << 9)
#define INT_B_SIG_EN								(1U << 10)
#define INT_C_SIG_EN								(1U << 11)
#define RE_TUNE_EVENT_SIG_EN						(1U << 12)
#define CQE_EVENT_SIG_EN							(1U << 14)
#define CMD_TOUT_ERR_SIG_EN							(1U << 16)
#define CMD_CRC_ERR_SIG_EN							(1U << 17)
#define CMD_ENDBIT_ERR_SIG_EN						(1U << 18)
#define CMD_IDX_ERR_SIG_EN							(1U << 19)
#define DAT_TOUT_ERR_SIG_EN							(1U << 20)
#define DAT_CRC_ERR_SIG_EN							(1U << 21)
#define DAT_ENDBIT_ERR_SIG_EN						(1U << 22)
#define CURR_LIMIT_ERR_SIG_EN						(1U << 23)
#define AUTO_CMD_ERR_SIG_EN							(1U << 24)
#define ADMA_ERR_SIG_EN								(1U << 25)
#define TUNE_ERR_SIG_EN								(1U << 26)
#define BOOT_ACK_ERR_SIG_EN							(1U << 28)

/* AUTO_CMD_ERR_AND_HOST_CTL2 */
#define GET_AUTO_CMD12_NO_EXE(x)					((x) & 0x1)
#define GET_AUTO_CMD_TOUT_ERR(x)					(((x) >> 1) & 0x1)
#define GET_AUTO_CMD_CRC_ERR(x)						(((x) >> 2) & 0x1)
#define GET_AUTO_CMD_ENDBIT_ERR(x)					(((x) >> 3) & 0x1)
#define GET_AUTO_CMD_IDX_ERR(x)						(((x) >> 4) & 0x1)
#define GET_CMD_NOT_ISSUE_BY_CMD12(x)				(((x) >> 7) & 0x1)
#define UHS_MODE_SEL(x)								((x) << 16)
#define EN_18_SIG									(1U << 19)
#define DRV_SEL(x)									((x) << 20)
#define EXECUTE_TUNE								(1U << 22)
#define SAMPLE_CLK_SEL								(1U << 23)
#define ASYNC_INT_EN								(1U << 30)
#define PRESET_VAL_ENABLE							(1U << 31)

/* CAPABILITIES1 */
#define GET_TOUT_CLK_FREQ(x)						((x) & 0x3F)
#define GET_TOUT_CLK_UNIT(x)						(((x) >> 7) & 0x1)
#define GET_BASE_CLK_FREQ(x)						(((x) >> 8) & 0xFF)
#define GET_MAX_BLK_LEN(x)							(((x) >> 16) & 0x3)
#define GET_EMBEDDED_8BIT(x)						(((x) >> 18) & 0x1)
#define GET_ADMA2_SUPPORT(x)						(((x) >> 19) & 0x1)
#define GET_HS_SUPPORT(x)							(((x) >> 21) & 0x1)
#define GET_SDMA_SUPPORT(x)							(((x) >> 22) & 0x1)
#define GET_SUSP_RES_SUPPORT(x)						(((x) >> 23) & 0x1)
#define GET_V33_SUPPORT(x)							(((x) >> 24) & 0x1)
#define GET_V30_SUPPORT(x)							(((x) >> 25) & 0x1)
#define GET_V18_SUPPORT(x)							(((x) >> 26) & 0x1)
#define GET_BUS64_SUPPORT(x)						(((x) >> 28) & 0x1)
#define GET_ASYNC_INT_SUPPORT(x)					(((x) >> 29) & 0x1)
#define GET_SLOT_TYPE(x)							(((x) >> 30) & 0x3)

/* CAPABILITIES2 */
#define GET_SDR50_SUPPORT(x)						((x) & 0x1)
#define GET_SDR104_SUPPORT(x)						(((x) >> 1) & 0x1)
#define GET_DDR50_SUPPORT(x)						(((x) >> 2) & 0x1)
#define GET_DRV_A_SUPPORT(x)						(((x) >> 4) & 0x1)
#define GET_DRV_C_SUPPORT(x)						(((x) >> 5) & 0x1)
#define GET_DRV_D_SUPPORT(x)						(((x) >> 6) & 0x1)
#define GET_RETUNE_TIMER(x)							(((x) >> 8) & 0xF)
#define GET_TUNE_SDR50(x)							(((x) >> 13) & 0x1)
#define GET_RETUNE_MODE(x)							(((x) >> 14) & 0x3)
#define GET_CLK_MULTIPLIER(x)						(((x) >> 16) & 0xFF)

/* FORCE_EVENT_ERR */
#define FORCE_AUTO_CMD12_NOT_EXE					(1U << 0)
#define FORCE_AUTO_CMD_TOUT_ERR						(1U << 1)
#define FORCE_AUTO_CMD_CRC_ERR						(1U << 2)
#define FORCE_AUTO_CMD_EBIT_ERR						(1U << 3)
#define FORCE_AUTO_CMD_IDX_ERR						(1U << 4)
#define FORCE_AUTO_CMD_NOT_ISSUE					(1U << 7)
#define FORCE_CMD_TOUT_ERR							(1U << 16)
#define FORCE_CMD_CRC_ERR							(1U << 17)
#define FORCE_CMD_EBIT_ERR							(1U << 18)
#define FORCE_CMD_IDX_ERR							(1U << 19)
#define FORCE_DAT_TOUT_ERR							(1U << 20)
#define FORCE_DAT_CRC_ERR							(1U << 21)
#define FORCE_DAT_EBIT_ERR							(1U << 22)
#define FORCE_CURR_LIMIT_ERR						(1U << 23)
#define FORCE_AUTO_CMD_ERR							(1U << 24)
#define FORCE_ADMA_ERR								(1U << 25)
#define FORCE_TUNING_ERR							(1U << 26)
#define FORCE_BOOT_ACK_ERR							(1U << 28)

/* ADMA_ERR_STS */
#define GET_ADMA_ERR_STS(x)							(((x) & 0x3))
#define GET_ADMA_LEN_MISMATCH(x)					((((x) >> 2) & 0x1))

/* SLOT_INT_AND_HOST_VER */
#define GET_INT_SLOT(x)								(((x) & 0xFF))
#define GET_SPEC_VER(x)								((((x) >> 16) & 0xFF))
#define GET_VENDOR_VER(x)							((((x) >> 24) & 0xFF))

/* EMMC_CTRL */
#define EMMC_FUNC_EN								(1U << 0)
#define LATANCY_1T									(1U << 1)
#define CLK_FREE_EN									(1U << 2)
#define DISABLE_DATA_CRC_CHK						(1U << 3)
#define EMMC_RSTN									(1U << 8)
#define EMMC_RSTN_OEN								(1U << 9)
#define CQE_ALGO_SEL								(1U << 12)
#define CQE_PREFETCH_DISABLE						(1U << 13)
#define timer_clk_sel								(1U << 16)

/* EMMC_BOOT_CTL */
#define BOOT_MODE_ENABLE							(1U << 0)
#define BOOT_ACK_ENABLE								(1U << 1)
#define BOOT_TOUT_CNT(x)							((x) << 4)
#define VALIDATE_BOOT								(1U << 8)

/* CDET_TOUT_CTL */
#define CDET_DEBUUNCE_CNT(x)						((x))

/* MBIU_CTRL */
#define UNDEFL_INCR_EN								(1U << 0)
#define BURST_INCR4_EN								(1U << 1)
#define BURST_INCR8_EN								(1U << 2)
#define BURST_INCR16_EN								(1U << 3)

/* PHY_TX_RX_DLY */
#define PHY_TX_DLY(x)								((x))
#define PHY_TX_SRC(x)								((x) << 8)
#define PHY_TX_EVEN_ODD								(1U << 10)
#define PHY_RX_DLY(x)								((x) << 16)
#define PHY_RX_SRC(x)								((x) << 24)
#define PHY_RX_EVEN_ODD								(1U << 26)

/* PHY_DS_DLY */
#define PHY_DS_DLY(x)								((x))
#define PHY_DS_SRC(x)								((x) << 8)
#define PHY_DS_EVEN_ODD								(1U << 10)

/* PHY_DLY_STS */
#define GET_PHY_TX_LEAD_LAG(x)						((x) & 0x1)
#define GET_PHY_RX_LEAD_LAG(x)						((x >> 1) & 0x1)
#define GET_PHY_DS_LEAD_LAG(x)						((x >> 2) & 0x1)

/* PHY_CONFIG */
#define PHY_TX_BPS									(1U << 0)
#define ADJ_TIMING_EN								(1U << 1)
#define ADJ_NCR(x)									((x) << 8)
#define ADJ_NCRC(x)									((x) << 10)

void sdSetClock(volatile struct SdmmcReg* sd, uint32_t div);
bool sdSendCMD(volatile struct SdmmcReg *sdmmc, uint32_t cmd, uint32_t argument);
bool init_sdmmc(uint64_t addr);
bool writeSDMMC(uint64_t sdmmcAddr, enum TransferMode type, uint64_t index, void *buff, uint64_t size);
bool readSDMMC(uint64_t sdmmcAddr, enum TransferMode type, uint64_t index, void *buff, uint64_t size);
bool errSDMMC(volatile struct SdmmcReg *sdmmcInit, const char* typeFail, uint32_t cmd);

#endif
