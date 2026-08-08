#ifndef __UART_H__
#define __UART_H__

#include "../mmio.h"
#include "../../../bootloader/DTB/platformDevices.h"

extern struct DtbPlatform globalDTB;

enum UARTmode {
	queryMode = 0,
	interruptMode,
	DMAMode
};

struct UartReg {
	volatile uint32_t RBR_THR_DLL;
	volatile uint32_t IER_DLH;
	volatile uint32_t FCR_IIR;
	volatile uint32_t LCR;
	volatile uint32_t MCR;
	volatile uint32_t LSR;
	volatile uint32_t MSR;

	volatile uint32_t RESERVED1;

	volatile uint32_t LPDLL;
	volatile uint32_t LPDLH;
	
	volatile uint32_t RESERVED2[2];

	volatile uint32_t SRBR_STHR;
	
	volatile uint32_t RESERVED3[15];
	
	volatile uint32_t FAR;
	volatile uint32_t TFR;
	volatile uint32_t RFW;
	volatile uint32_t USR;
	volatile uint32_t TFL;
	volatile uint32_t RFL;
	volatile uint32_t SRR;
	volatile uint32_t SRTS;
	volatile uint32_t SBCR;
	volatile uint32_t SDMAM;
	volatile uint32_t SFE;
	volatile uint32_t SRT;
	volatile uint32_t STET;
	volatile uint32_t HTX;
	volatile uint32_t DMASA;
};

void init_uart(uint64_t uartAddr, enum UARTmode mode);
void uartWrite(uint64_t uartAddr, uint8_t symbol);
uint8_t uartRead(uint64_t uartAddr); 

#endif
