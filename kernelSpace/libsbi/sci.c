/* sbi console interface */

#include "../../include/kernelSpace/libsbi/sci.h"

sbi_ret_t sbi_console_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    #ifdef __DBCN__
        struct sbiret err;
    #else
        int err;
    #endif

    while(*format) {
        if(*format == '%') {
            ++format;
            switch(*format) {
                case 'd':
                    {
                        int value = va_arg(args, int);
                        if(value < 0) {
                            #ifdef __DBCN__
                                if((err = sbi_debug_console_write_byte('-')).error != 0) return err;
                            #else
                                if((err = sbi_console_putchar('-')) != 0) return err;
                            #endif

                            value = -value;
                        }
                        
                        int div = 1;
                        while(value / div > 9) div *= 10;

                        while(div) {
                            #ifdef __DBCN__
                                if((err = sbi_debug_console_write_byte((value / div % 10) | 0x30)).error != 0) return err;
                            #else
                                if((err = sbi_console_putchar((value / div % 10) | 0x30)) != 0) return err;
                            #endif

                            div /= 10;
                        }
                        break;
                    }

                case 's':
                    {
                        const char* str = va_arg(args, const char*);
                        while(*str) {
                            #ifdef __DBCN__
                                if((err = sbi_debug_console_write_byte(*str)).error != 0) return err;
                            #else
                                if((err = sbi_console_putchar(*str)) != 0) return err;
                            #endif

                            ++str;
                        }
                        break;
                    }
                
                case 'c':
                    {
                        int s = va_arg(args, int);
                        #ifdef __DBCN__
                            if((err = sbi_debug_console_write_byte(s)).error != 0) return err;
                        #else
                            if((err = sbi_console_putchar(s)) != 0) return err;
                        #endif

                        break;
                    }
            }
        }
        else {
            #ifdef __DBCN__
                if((err = sbi_debug_console_write_byte(*format)).error != 0) return err;
            #else
                if((err = sbi_console_putchar(*format)) != 0) return err;
            #endif
        }
        ++format;
    }
    va_end(args);

    return err;
}
