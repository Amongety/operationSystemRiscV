#include "../include/kernelSpace/config.h"
#include "../include/kernelSpace/arch/riscV/kernel_init.h"

void main() 
{
	uint32_t *root_page_table = (uint32_t*)alloc_pages(1);

	kernel_init(root_page_table);
	
        sbi_console_printf("Hello debug SBI!\n");

	while(1) {}

	/*
	__asm__ __volatile__("unimp");
	
	PANIC("booted");

        for(int i = 0; i < 5; ++i) {
                sbi_console_putchar("Test\n"[i]);
        }

        sbi_debug_console_write_byte('a');

        unsigned char test[8];
        struct sbiret ret = sbi_debug_console_read(6, (unsigned long*)test);
        if(ret.uvalue == 6) {
                test[6] = '!';
                test[7] = '\n';
                sbi_debug_console_write(8, (unsigned long*)test);
        }
        */

}
