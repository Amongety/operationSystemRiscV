#include "../../../include/kernelSpace/drivers/sdmmc/sdmmc.h"

static uint32_t RCA = 0;
static uint32_t ocr = 0;

void sdSetClock(volatile struct SdmmcReg* sd) {
	sd->CLK_CTL_SWRST &= ~SD_CLK_EN;

    	sd->CLK_CTL_SWRST |= INT_CLK_EN;
    	while(!(GET_INT_CLK_STABLE(sd->CLK_CTL_SWRST))) {}

    	sd->CLK_CTL_SWRST &= ~FREQ_SEL(0xFF);
    	sd->CLK_CTL_SWRST |= FREQ_SEL(0x80);

    	sd->CLK_CTL_SWRST |= SD_CLK_EN;
}

bool sdSendCMD(volatile struct SdmmcReg *sdmmc, uint32_t cmd, uint32_t argument) {
	while(GET_CMD_INHIBIT(sdmmc->PRESENT_STS)) {}

	if((cmd & DATA_PRESENT_SEL) && (GET_CMD_IDX(cmd) != 12)) {
		while(GET_CMD_INHIBIT_DAT(sdmmc->PRESENT_STS)) {}
	}

	sdmmc->NORM_AND_ERR_INT_STS = sdmmc->NORM_AND_ERR_INT_STS;

    	sdmmc->ARGUMENT = argument;
    	sdmmc->XFER_MODE_AND_CMD = cmd;

	for(int i = 0; i < TIME_WAIT; ++i) {
		if(errSDMMC(sdmmc, "CMD error")) return false;

       		if(sdmmc->NORM_AND_ERR_INT_STS & CMD_CMPL) {
			sdmmc->NORM_AND_ERR_INT_STS = CMD_CMPL;
			break;
		}
    	}

	if((GET_CMD_IDX(cmd) != 25) && (cmd & DATA_PRESENT_SEL)) {
		for(int i = 0; i < TIME_WAIT; ++i) {
			if(errSDMMC(sdmmc, "DATA error")) return false;

            		if(sdmmc->NORM_AND_ERR_INT_STS & XFER_CMPL) {
                		sdmmc->NORM_AND_ERR_INT_STS = XFER_CMPL;
                		break;
            		}
		}
	}

	return true;
}

bool init_sdmmc(uint64_t addr) {
	volatile struct SdmmcReg* sdmmc = (struct SdmmcReg*)addr;

    	sdSetClock(sdmmc);

    	sdmmc->HOST_CTL1_PWR_BG_WUP = SD_BUS_PWR | SD_BUS_VOL_SEL(0x7);
	for(int i = 0; i < 1000; i++) {}

    	while(GET_CMD_INHIBIT(sdmmc->PRESENT_STS)) {}

    	sdSendCMD(sdmmc, CMD_IDX(0), 0);

    	sdSendCMD(sdmmc,  RESP_TYPE_SEL(2) | CMD_CRC_CHK_ENABLE | CMD_IDX_CHK_ENABLE | CMD_IDX(8), 0x1AA);

    	if((sdmmc->RESP31_0 & 0x1FF) != 0x1AA) {
        	console_printf("CMD8 FAIL: %x\r\n", sdmmc->RESP31_0);
        	return false;
    	}

    	do {
        	sdSendCMD(sdmmc, RESP_TYPE_SEL(2) | CMD_CRC_CHK_ENABLE | CMD_IDX_CHK_ENABLE | CMD_IDX(55), 0);
        	sdSendCMD(sdmmc, RESP_TYPE_SEL(2) | CMD_IDX(41), 0x40300000);
    	} while(!(sdmmc->RESP31_0 & (1U << 31)));

    	ocr = sdmmc->RESP31_0;

    	sdSendCMD(sdmmc, RESP_TYPE_SEL(1) | CMD_IDX(2), 0);

    	sdSendCMD(sdmmc, RESP_TYPE_SEL(2) | CMD_CRC_CHK_ENABLE | CMD_IDX_CHK_ENABLE | CMD_IDX(3), 0);

    	RCA = (sdmmc->RESP31_0 >> 16);

    	sdSendCMD(sdmmc, RESP_TYPE_SEL(3) | CMD_IDX(7), RCA << 16);
	
	while(GET_CMD_INHIBIT_DAT(sdmmc->PRESENT_STS)) {}

	sdSendCMD(sdmmc, RESP_TYPE_SEL(2) | CMD_CRC_CHK_ENABLE | CMD_IDX_CHK_ENABLE | CMD_IDX(13), RCA << 16);
	if(((sdmmc->RESP31_0 >> 9) & 0xF) != 4) {
		console_printf("CMD13 fail:\r\nCMD13 RESP = %x\r\nSTATE = %d\r\n", sdmmc->RESP31_0, (sdmmc->RESP31_0 >> 9) & 0xF);
		return false;
	}
	
    	sdSendCMD(sdmmc, RESP_TYPE_SEL(2) | CMD_CRC_CHK_ENABLE | CMD_IDX_CHK_ENABLE | CMD_IDX(16), 512);
	
    	return true;
}

bool writeSDMMC(uint64_t sdmmcAddr, enum TransferMode type, uint64_t index, void *buff, uint64_t size) {
	volatile struct SdmmcReg* sdmmcInit = (struct SdmmcReg*)sdmmcAddr;

	switch(type) {
		case NONDMA: {

			uint32_t blocks = (size + 511) / 512;
			uint32_t words  = size / 4;
			uint64_t pos = 0;
			uint32_t arg = index;

			if(!((ocr >> 30) & 1)) arg *= 512;

			sdmmcInit->HOST_CTL1_PWR_BG_WUP &= ~(DMA_SEL(0x3));
			sdmmcInit->HOST_CTL1_PWR_BG_WUP |= DMA_SEL(0x1);
			for(int i = 0; i < 1000; i++) {}

			sdmmcInit->BLK_SIZE_AND_CNT = 512 | (blocks << 16);

			sdSendCMD(sdmmcInit, BLK_CNT_ENABLE | MULTI_BLK_SEL | RESP_TYPE_SEL(2) | CMD_CRC_CHK_ENABLE | CMD_IDX_CHK_ENABLE | DATA_PRESENT_SEL | CMD_IDX(25), arg);

			uint32_t *data = (uint32_t*)buff;

			for(int b = 0; b < blocks; ++b) {
				if(errSDMMC(sdmmcInit, "Error transfer data")) return false;
				
				while(!(sdmmcInit->NORM_AND_ERR_INT_STS & BUF_WRDY)) {}
				sdmmcInit->NORM_AND_ERR_INT_STS = BUF_WRDY;
					
				for(int i = 0; i < 128; ++i) {
					sdmmcInit->BUF_DATA = (pos < words) ? data[pos++]:0;
				}
			}

			while((GET_DAT_LINE_ACTIVE(sdmmcInit->PRESENT_STS))) {}

			sdSendCMD(sdmmcInit, RESP_TYPE_SEL(2) | CMD_CRC_CHK_ENABLE | CMD_IDX_CHK_ENABLE | CMD_IDX(12), 0);

			while(GET_CMD_INHIBIT_DAT(sdmmcInit->PRESENT_STS)) {}

			break;
		}
		case SDMA:
			break;
		case ADMA:
			break;
	};

	return true;
}

bool readSDMMC(uint64_t sdmmcAddr, enum TransferMode type, uint64_t index, void *buff, uint64_t size) {
	volatile struct SdmmcReg* sdmmcInit = (struct SdmmcReg*)sdmmcAddr;

	switch(type) {
		case NONDMA: {
			uint32_t blocks = (size + 511) / 512;
			uint32_t words  = size / 4;
			uint64_t pos = 0;
			uint32_t arg = index;

			if(!((ocr >> 30) & 1)) arg *= 512;

			sdmmcInit->HOST_CTL1_PWR_BG_WUP &= ~(DMA_SEL(0x3));
			sdmmcInit->HOST_CTL1_PWR_BG_WUP |= DMA_SEL(0x1);
			for(int i = 0; i < 1000; i++) {}

			sdmmcInit->BLK_SIZE_AND_CNT = 512 | (blocks << 16);

			sdSendCMD(sdmmcInit, BLK_CNT_ENABLE | DAT_XFER_DIR | MULTI_BLK_SEL | RESP_TYPE_SEL(2) | CMD_CRC_CHK_ENABLE | CMD_IDX_CHK_ENABLE | DATA_PRESENT_SEL | CMD_IDX(18), arg);

			uint32_t *data = (uint32_t*)buff;

			for(int b = 0; b < blocks; ++b) {
				if(errSDMMC(sdmmcInit, "Error read data")) return false;

				while(!(sdmmcInit->NORM_AND_ERR_INT_STS & BUF_RRDY)) {}
				sdmmcInit->NORM_AND_ERR_INT_STS = BUF_RRDY;
				
				for (int i = 0; i < 128; ++i) {
					if(pos < words) data[pos++] = sdmmcInit->BUF_DATA;
					else sdmmcInit->BUF_DATA;
				}
			}
			
			while((GET_DAT_LINE_ACTIVE(sdmmcInit->PRESENT_STS))) {}

			sdSendCMD(sdmmcInit, RESP_TYPE_SEL(2) | CMD_CRC_CHK_ENABLE | CMD_IDX_CHK_ENABLE | CMD_IDX(12), 0);
			
			while(GET_CMD_INHIBIT_DAT(sdmmcInit->PRESENT_STS)) {}

			break;
		}
		case SDMA:
			break;
		case ADMA:
			break;
	};

	return true;
}

bool errSDMMC(volatile struct SdmmcReg* sdmmcInit, const char* typeFail) {
	if(sdmmcInit->NORM_AND_ERR_INT_STS & ERR_INT_STS_ERR_MASK) {
		console_printf("Error SD_CARD: \'%s\'\r\n", typeFail);
		console_printf("RESPONSE:\r\nRESP31_0:%x\r\nRESP63_32:%x\r\nRESP95_64:%x\r\nRESP127_96:%x\r\n", sdmmcInit->RESP31_0, sdmmcInit->RESP63_32, sdmmcInit->RESP95_64, sdmmcInit->RESP127_96);
		if(sdmmcInit->NORM_AND_ERR_INT_STS & CMD_TOUT_ERR) console_printf("CMD Timeout Error\r\n");
		if(sdmmcInit->NORM_AND_ERR_INT_STS & CMD_CRC_ERR) console_printf("CMD CRC Error\r\n");
		if(sdmmcInit->NORM_AND_ERR_INT_STS & CMD_ENDBIT_ERR) console_printf("CMD End Bit Error\r\n");
		if(sdmmcInit->NORM_AND_ERR_INT_STS & CMD_IDX_ERR) console_printf("CMD Index Error\r\n");
		if(sdmmcInit->NORM_AND_ERR_INT_STS & DAT_TOUT_ERR) console_printf("DATA Timeout Error\r\n");
		if(sdmmcInit->NORM_AND_ERR_INT_STS & DAT_CRC_ERR) console_printf("DATA CRC Error\r\n");
		if(sdmmcInit->NORM_AND_ERR_INT_STS & DAT_ENDBIT_ERR) console_printf("DATA End Bit Error\r\n");
		if(sdmmcInit->NORM_AND_ERR_INT_STS & CURR_LIMIT_ERR) console_printf("DATA Limit Error\r\n");
		if(sdmmcInit->NORM_AND_ERR_INT_STS & AUTO_CMD_ERR) console_printf("Auto CMD Error\r\n");
		if(sdmmcInit->NORM_AND_ERR_INT_STS & ADMA_ERR) console_printf("ADMA Error\r\n");
		if(sdmmcInit->NORM_AND_ERR_INT_STS & TUNE_ERR) console_printf("Tuning Error\r\n");
		if(sdmmcInit->NORM_AND_ERR_INT_STS & BOOT_ACK_ERR) console_printf("Boot Ack Error\r\n");
		return true;
	}

	return false;
}
