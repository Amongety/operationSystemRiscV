#include "../../include/kernelSpace/debug/kdebug.h"

console_ret_t console_printf(const unsigned char* format, ...) {
    va_list args;
    va_start(args, format);

    #if defined(__UART_USE0__)
       int err;
    #elif defined(__DBCN__)
        struct sbiret err;
    #elif defined(__CN__)
        int err;
    #endif

    while(*format) {
        if(*format == '%') {
            ++format;
            switch(*format) {
                case 'd':
                    {
                        uint64_t value = va_arg(args, uint64_t);
                        if(value < 0) {
			    #if defined(__UART_USE0__)
				uartWrite(UART_ADDR, '-');	
			    #elif defined(__DBCN__)
                                if((err = sbi_debug_console_write_byte('-')).error != 0) return err;
                            #elif defined(__CN__)
                                if((err = sbi_console_putchar('-')) != 0) return err;
                            #endif

                            value = -value;
                        }
                        
                        uint64_t div = 1;
                        while(value / div > 9) div *= 10;

                        while(div) {
			    #if defined(__UART_USE0__)
				uartWrite(UART_ADDR, (value / div % 10) | 0x30);
                            #elif defined(__DBCN__)
                                if((err = sbi_debug_console_write_byte((value / div % 10) | 0x30)).error != 0) return err;
                            #elif defined(__CN__)
                                if((err = sbi_console_putchar((value / div % 10) | 0x30)) != 0) return err;
                            #endif

                            div /= 10;
                        }
                        break;
                    }

		case 'x': 
		    {
			uint64_t value = va_arg(args, uint64_t);
                    	for (int i = 15; i >= 0; i--) {
                        	uint64_t nibble = (value >> (i * 4)) & 0xf;
				#if defined(__UART_USE0__)
					uartWrite(UART_ADDR, "0123456789abcdef"[nibble]);
				#elif defined(__DBCN__)
					if((err = sbi_debug_console_write_byte("0123456789abcdef"[nibble]))).error != 0) return err;
				#elif defined(__CN__)
                        		if((err = sbi_console_putchar("0123456789abcdef"[nibble])) != 0) return err;
				#endif
                    	}
			break;
		    }

                case 's':
                    {
                        const unsigned char* str = va_arg(args, const unsigned char*);
                        while(*str) {
			    #if defined(__UART_USE0__)
				uartWrite(UART_ADDR, *str);
                            #elif defined(__DBCN__)
                                if((err = sbi_debug_console_write_byte(*str)).error != 0) return err;
                            #elif defined(__CN__)
                                if((err = sbi_console_putchar(*str)) != 0) return err;
                            #endif

                            ++str;
                        }
                        break;
                    }
                
                case 'c':
                    {
                        int s = va_arg(args, int);
			#if defined(__UART_USE0__)
				uartWrite(UART_ADDR, s);
                        #elif defined(__DBCN__)
                            if((err = sbi_debug_console_write_byte(s)).error != 0) return err;
                        #elif defined(__CN__)
                            if((err = sbi_console_putchar(s)) != 0) return err;
                        #endif

                        break;
                    }
            }
        }
        else {
	    #if defined(__UART_USE0__)
		uartWrite(UART_ADDR, *format);
            #elif defined(__DBCN__)
                if((err = sbi_debug_console_write_byte(*format)).error != 0) return err;
            #elif defined(__CN__)
                if((err = sbi_console_putchar(*format)) != 0) return err;
            #endif
        }
        ++format;
    }
    va_end(args);

    return err;
}

uint8_t console_getchar(void) {
	return uartRead(UART_ADDR);
}

