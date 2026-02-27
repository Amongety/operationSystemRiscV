#ifndef __UART_H__
#define __UART_H__

#include "../mmio.h"

enum UARTmode{
	queryMode = 0,
	interruptMode,
	DMAMode
};

struct UartReg {
	volatile uint8_t RBR_THR_DLL;
	volatile uint8_t IER_DLH;
	volatile uint8_t FCR_IIR;
	volatile uint8_t LCR;
	volatile uint8_t MCR;
	volatile uint8_t LSR;
	volatile uint8_t MSR;
	volatile uint8_t LPDLL;
	volatile uint8_t LPDLH;
	volatile uint8_t SRBR_STHR;
	volatile uint64_t FAR:1;
	volatile uint64_t TFR:8;
	volatile uint64_t RFW:10;
	volatile uint64_t USR:5;
	volatile uint64_t TFL:6;
	volatile uint64_t RFL:6;
	volatile uint64_t SRR:3;
	volatile uint64_t SRTS:1;
	volatile uint64_t SBCR:1;
	volatile uint64_t SDMAM:1;
	volatile uint64_t SFE:1;
	volatile uint64_t SRT:2;
	volatile uint64_t STET:2;
	volatile uint64_t HTX:1;
	volatile uint64_t DMASA:1;
	volatile uint64_t RESERVED:13;
};

void init_uart(uint64_t uartAddr, enum UARTmode mode);
void uartWrite(uint64_t uartAddr, uint8_t symbol);
uint8_t uartRead(uint64_t uartAddr); 

#endif
