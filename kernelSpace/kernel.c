#include "../include/kernelSpace/config.h"
#include "../include/kernelSpace/arch/riscV/kernel_init.h"

void main() 
{
	kernel_init();

        sbi_console_printf("Hello debug SBI!\n");
	
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
