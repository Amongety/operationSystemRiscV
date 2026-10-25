// #include "../include/userSpace/user_test.h"
#include "../include/userSpace/sys/ioctl.h"
#include <stdint.h>
#include <stdarg.h>

void printfUART(uint64_t device, const unsigned char* format, ...);

#define SIZE 1000
int myarray[SIZE];
int another_array[5] = {1, 2, 3, 4, 5};

int main() {
	// struct GpioArg gpio = {.pin=29,.mode=GPIO_OUTPUT,.value=0};
	// ioctl(DEVICE_GPIO0, GPIO_SET_MODE, (void*)&gpio);

	// while(true) {
	// 	gpio.value = 0;
	// 	printfUART(DEVICE_UART0, "Set %d\r\n", 0);
	// 	ioctl(DEVICE_GPIO0, GPIO_WRITE, (void*)&gpio);
	// 	for(int i = 0; i < 5000000; ++i) {}
	// 	printfUART(DEVICE_UART0, "Set %d\r\n", 1);
	// 	gpio.value = 1;
		
	// 	ioctl(DEVICE_GPIO0, GPIO_WRITE, (void*)&gpio);
	// 	for(int i = 0; i < 5000000; ++i) {}

	// }
	
    printfUART(DEVICE_UART0, "My array is at %x\r\n", (unsigned long)myarray);
	
    printfUART(DEVICE_UART0, "I'm going to start crunching some numbers, so gimme a minute.\r\n", 0);
	
	
    for(int i = 0; i < SIZE; ++i) {
		myarray[i] = another_array[i % 5];
    }
	
    for(int i = 0; i < 100000000; ++i) {
		myarray[i % SIZE] += 1;
    }
	
	
    printfUART(DEVICE_UART0, "Ok, I'm done crunching. Wanna see myarray[0]? It's %d\r\n", myarray[0]);
	
	return 0;
}

void printfUART(uint64_t device, const unsigned char* format, ...) {
	unsigned char minus = '-', c;
	va_list args;
   	va_start(args, format);

    while(*format) {
        if(*format == '%') {
            ++format;
            switch(*format) {
                case 'd':
                {
                    long long value = va_arg(args, long long);
                    if(value < 0) {
						ioctl(device, UART_WRITE, (void*)minus);
                        value = -value;
                    }
                        
                    uint64_t div = 1;
                    while(value / div > 9) div *= 10;

                    while(div) {
						uint64_t t = ((value / div % 10) | 0x30);
						ioctl(device, UART_WRITE, (void*)(&t));
                        div /= 10;
                    }
                    break;
               	}

				case 'x': 
		    	{
					uint64_t value = va_arg(args, uint64_t);
                    for (int i = 15; i >= 0; i--) {
                    	uint64_t nibble = (value >> (i * 4)) & 0xf;
						c = "0123456789abcdef"[nibble];
						ioctl(device, UART_WRITE, (void*)&c);
                    }
					break;
		    	}

                case 's':
                {
                    const unsigned char* str = va_arg(args, const unsigned char*);
                    while(*str) {
						ioctl(device, UART_WRITE, (void*)str);
                        ++str;
                    }
                    break;
                }
                
            case 'c':
            {
                int s = va_arg(args, int);
				ioctl(device, UART_WRITE, (void*)&s);
        	}
			};
		}
    	else ioctl(device, UART_WRITE, (void*)format);
    	++format;
    }
    va_end(args);
}