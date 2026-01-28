#include "../include/kernelSpace/config.h"
#include "../include/kernelSpace/arch/riscV/kernel_init.h"
#include "../include/kernelSpace/process/process_control.h"
#include "../include/kernelSpace/libsbi/te.h"

extern unsigned char _data_start[];
extern unsigned char _data_end[];
extern unsigned char _bss_start[];
extern unsigned char _bss_end[];
extern unsigned char _stack[];
extern unsigned char _text_start[];
extern unsigned char _text_end[];
extern unsigned char _rodata_start[];
extern unsigned char _rodata_end[];

//extern void main();

void main() 
{
	uint32_t *root_page_table = (uint32_t*)alloc_pages(1);

	kernel_init(root_page_table);

	sbi_console_printf("Text %x %x\n", _text_start, _text_end);
	sbi_console_printf("Rodata %x %x\n", _rodata_start, _rodata_end);
	sbi_console_printf("Data %x %x\n", _data_start, _data_end);
	sbi_console_printf("Bss %x %x\n", _bss_start, _bss_end);
	sbi_console_printf("Stack %x %x\n", _stack - 128 * 1024, _stack);
	sbi_console_printf("FreeRam %x %x\n", _free_ram_start, _free_ram_end);

	sbi_console_printf("Hello debug SBI!\n");

	//create_process((unsigned long)main);
	sbi_set_timer(100000000);

	while(1) {
		asm("wfi");
	}
}
