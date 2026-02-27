#include "../../../include/kernelSpace/drivers/uart/uart.h"

void init_uart(uint64_t uartAddr, enum UARTmode mode) {
	volatile struct UartReg *UARTinit = (struct UartReg*)uartAddr;
	
	switch(mode) {
		case queryMode: {
			UARTinit->LCR |= 0x80;
			uint16_t divisor = 592;
			UARTinit->RBR_THR_DLL = (uint8_t)divisor & 0xff;
			UARTinit->IER_DLH = (uint8_t)divisor >> 8;
			UARTinit->LCR &= (~0x80);
			UARTinit->LCR = 0x3;
			UARTinit->FCR_IIR = 0xc0;
			break;
		}

		case interruptMode: {
			UARTinit->LCR |= 0x80;
			uint16_t divisor = 592;
			UARTinit->RBR_THR_DLL = (uint8_t)divisor & 0xff;
			UARTinit->IER_DLH = (uint8_t)divisor >> 8;
			UARTinit->LCR &= (~0x80);
			UARTinit->LCR = 0x3;
			UARTinit->FCR_IIR = 0xc0;
			UARTinit->IER_DLH |= 0x3;
			break;
		}
		
		case DMAMode: {
			UARTinit->LCR |= 0x80;
			uint16_t divisor = 592;
			UARTinit->RBR_THR_DLL = (uint8_t)divisor & 0xff;
			UARTinit->IER_DLH = (uint8_t)divisor >> 8;
			UARTinit->LCR &= (~0x80);
			UARTinit->FCR_IIR = 0x7;
			break;
		}
	};
}

void uartWrite(uint64_t uartAddr, uint8_t symbol) {
	volatile struct UartReg *uart = (struct UartReg*)uartAddr;

	if(!(uart->LCR & 0x80)) uart->RBR_THR_DLL = symbol;

	//while(uart->USR & 0x4) {}
}

uint8_t uartRead(uint64_t uartAddr) {
	volatile struct UartReg *uart = (struct UartReg*)uartAddr;
	
	//while(!(uart->USR & 0x8)) {}
	while(!(uart->LSR & 0x1)) {}

	return uart->RBR_THR_DLL;
}

