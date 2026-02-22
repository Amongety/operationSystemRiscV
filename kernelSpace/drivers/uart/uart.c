#include "../../../include/kernelSpace/drivers/uart/uart.h"

void init_uart() {
	volatile struct UartReg *UARTinit = (struct UartReg*)UART_ADDR;

	UARTinit->LCR |= 0x80;
	uint16_t divisor = 592;
	UARTinit->RBR_THR_DLL = (uint8_t)divisor & 0xff;
	UARTinit->IER_DLH = (uint8_t)divisor >> 8;
	UARTinit->LCR &= (~0x80);
	UARTinit->LCR = 0x3;
	UARTinit->FCR_IIR = 0xc0;
}

void uartWrite(uint8_t symbol) {
	volatile struct UartReg *uart = (struct UartReg*)UART_ADDR;

	if(!(uart->LCR & 0x80)) uart->RBR_THR_DLL = symbol;

	//while(uart->USR & 0x4) {}
}

uint8_t uartRead() {
	volatile struct UartReg *uart = (struct UartReg*)UART_ADDR;
	
	//while(!(uart->USR & 0x8)) {}
	while(!(uart->LSR & 0x1)) {}

	return uart->RBR_THR_DLL;
}

