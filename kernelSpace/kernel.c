#include "../include/kernelSpace/config.h"
#include "../include/kernelSpace/arch/riscV/kernel_init.h"
#include "../include/kernelSpace/libsbi/te.h"

void main() 
{
	kernel_init();
	
	static uint64_t* root_page_table;
	root_page_table = (uint64_t*)alloc_pages(1);

	init_virtual_memory(root_page_table);

	init_uart();
	
	console_printf("Text %x %x\r\n", _text_start, _text_end);
	console_printf("Rodata %x %x\r\n", _rodata_start, _rodata_end);
	console_printf("Data %x %x\r\n", _data_start, _data_end);
	console_printf("Bss %x %x\r\n", _bss_start, _bss_end);
	console_printf("Stack %x %x\r\n", _stack - 128 * 1024, _stack);
	console_printf("FreeRam %x %x\r\n", _free_ram_start, _free_ram_end);

	console_printf("Hello debug SBI!\r\n");

	create_process(0x20000000); // userSpace/user_test.c	 void main(void)
	create_process(0x20000000);
	create_process(0x20000000); 
	sbi_set_timer(1);

	while(1) {
		asm("wfi");
	}
}

