// #include "../include/userSpace/user_test.h"
#include "../include/userSpace/sys/ioctl.h"
#include <stdint.h>
#include <stdarg.h>

void printfUART(uint64_t device, const unsigned char* format, ...);

#define AHT20_ADDR 0x38
int aht20_init(uint64_t addr, uint64_t addrSlave);
int aht20_read(uint64_t addr, int32_t* temperature, uint32_t* humidity);

#define BMP280_ADDR 0x77
struct BMP280 {
	uint16_t T1;
	int16_t T2, T3;

	uint16_t P1;
	int16_t P2, P3, P4, P5;
	int16_t P6, P7, P8, P9;
};
static uint16_t bmpU16(uint8_t* data);
static int16_t bmpS16(uint8_t* data);
int bmp280_init(uint64_t addr, uint64_t addrSlave, struct BMP280* bmp);
int bmp280_read(uint64_t addr, struct BMP280* bmp, int32_t* temperature, uint32_t* pressure);

#define LCD2004A_ADDR 0x27
void lcdPulseEnable(uint64_t addr, char data); // Импульс на выводе Enable (EN)
void lcdSend4Bits(uint64_t addr, char nibble, char mode); // Отправка 4 бит данных/команды
void lcdSendByte(uint64_t addr, char value, char mode); // Отправка полного байта (делится на 2 посылки по 4 бита)
void lcdCommand(uint64_t addr, char cmd); // Отправить команду (RS = 0)
void lcdData(uint64_t addr, char data); // Отправить символ/данные (RS = 1)
void lcdPrint(uint64_t addr, const char* str); // Вывод строки
void lcdSetCursor(uint64_t addr, char col, char row); // Установка курсора (колонка 0..19, строка 0..3)

#define SIZE 1000
int myarray[SIZE];
int another_array[5] = {1, 2, 3, 4, 5};

int main() {
    /* AHT20 */
    /*int ret = aht20_init(DEVICE_I2C1, AHT20_ADDR);
	printfUART(DEVICE_UART0, "AHT20 init: %d\r\n", ret);
	if(ret == 0) {
		int32_t temperature;
		uint32_t humidity;
		
		ret = aht20_read(DEVICE_I2C1, &temperature, &humidity);
		if(ret == 0) printfUART(DEVICE_UART0, "AHT20 T*100 = %d, RH*100 = %d\r\n", temperature, humidity);
		else printfUART(DEVICE_UART0, "AHT20 read error: %d\r\n", ret);
	}*/

    /* BMP280 */
    /*struct BMP280 bmp;
	ret = bmp280_init(DEVICE_I2C1, BMP280_ADDR, &bmp);
	printfUART(DEVICE_UART0, "BMP280 INIT = %d\r\n", ret);

	if(ret == 0) {
		int32_t temperature = 0;
		uint32_t pressure = 0;

		ret = bmp280_read(DEVICE_I2C1, &bmp, &temperature, &pressure);

		if(ret == 0) printfUART(DEVICE_UART0, "BMP280 TEMP*100 = %d\r\n""BMP280 PRESSURE = %d Pa\r\n", temperature, pressure);
		else printfUART(DEVICE_UART0, "BMP280 READ ERROR = %d\r\n", ret);
	}*/

    /* LCD 2400A */
    struct I2cArg i2c = {.mode=MASTER,.addr=LCD2004A_ADDR};
    ioctl(DEVICE_I2C1, I2C_SET_MODE, (void*)&i2c);
	lcdSend4Bits(DEVICE_I2C1, 0x30, 0);
	// delay(5);
	for(int i = 0; i < 500000; ++i) {};
	lcdSend4Bits(DEVICE_I2C1, 0x30, 0);
	// delayMicroseconds(150);
	for(int i = 0; i < 150000; ++i) {};
	lcdSend4Bits(DEVICE_I2C1, 0x30, 0);
	lcdSend4Bits(DEVICE_I2C1, 0x20, 0); // Переход в 4-битный режим
	lcdCommand(DEVICE_I2C1, 0x28); // 2 строки (для логики дисплея), 5x8 матрица
	lcdCommand(DEVICE_I2C1, 0x0C); // Дисплей включен, курсор выключен
	lcdCommand(DEVICE_I2C1, 0x06); // Автоинкремент курсора
	lcdCommand(DEVICE_I2C1, 0x01); // Очистка дисплея
	// delay(2);
	for(int i = 0; i < 200000; ++i) {};
	// Вывод текста
	lcdSetCursor(DEVICE_I2C1, 0, 0);
	lcdPrint(DEVICE_I2C1, "Hello, LCD 2004!");
	lcdSetCursor(DEVICE_I2C1, 0, 1);
	lcdPrint(DEVICE_I2C1, "I2C without lib.");
	lcdSetCursor(DEVICE_I2C1, 0, 2);
	lcdPrint(DEVICE_I2C1, "Line 3: OK");
	lcdSetCursor(DEVICE_I2C1, 0, 3);
	lcdPrint(DEVICE_I2C1, "Line 4: Ready");
	for(int i = 0; i < 5000000; ++i) {};
	lcdCommand(DEVICE_I2C1, 0x01);

	uint8_t tx = 0x00;
    uint8_t rx = 0;
    // Первый тест: P4 = 0
    i2c.reg=tx; i2c.rData=(void*)&rx; i2c.rLen=1;
    ioctl(DEVICE_I2C1, I2C_READ_REG, (void*)&i2c);
    printfUART(DEVICE_UART0, "\r\nTX = %x RX = %x\r\n", tx, rx);
    if(rx & 0x10) printfUART(DEVICE_UART0, "TEST 1 FAILED\r\n");
    // Второй тест: P4 = 1
    tx = 0x10;
    i2c.reg=tx; i2c.rData=(void*)&rx; i2c.rLen=1;
    ioctl(DEVICE_I2C1, I2C_READ_REG, (void*)&i2c);
    printfUART(DEVICE_UART0, "TX = %x RX = %x\r\n", tx, rx);
    if(rx & 0x10) printfUART(DEVICE_UART0, "I2C READ TEST OK\r\n");
    else printfUART(DEVICE_UART0, "TEST 2 FAILED\r\n");

    /* GPIO */
	struct GpioArg gpio = {.pin=28,.mode=GPIO_OUTPUT};
	ioctl(DEVICE_GPIO0, GPIO_SET_MODE, (void*)&gpio);

	for(int i = 0; i < 10; ++i) {
		printfUART(DEVICE_UART0, "Set %d\r\n", 0);
		gpio.value = 0;
		ioctl(DEVICE_GPIO0, GPIO_WRITE, (void*)&gpio);
		for(int i = 0; i < 5000000; ++i) {}
		
        printfUART(DEVICE_UART0, "Set %d\r\n", 1);
		gpio.value = 1;
		ioctl(DEVICE_GPIO0, GPIO_WRITE, (void*)&gpio);
		for(int i = 0; i < 5000000; ++i) {}
	}
	
    /* Test memory */
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
						ioctl(device, UART_WRITE, (void*)&minus);
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

/* LCD 2004A */
// Импульс на выводе Enable (EN)
void lcdPulseEnable(uint64_t addr, char data) {
    char t = data | 0x04;
    struct I2cArg i2c = {.buf=(void*)&t,.lenData=1};
    ioctl(addr, I2C_WRITE, (void*)(&i2c));
    // delayMicroseconds(1);

	t = data & ~0x04;

    i2c.buf=(void*)&t;
    ioctl(addr, I2C_WRITE, (void*)(&i2c));
  	// delayMicroseconds(50);
}

// Отправка 4 бит данных/команды
void lcdSend4Bits(uint64_t addr, char nibble, char mode) {
  	char data = (nibble & 0xF0) | mode | 0x08;
    struct I2cArg i2c = {.buf=(void*)&data,.lenData=1};
  	ioctl(addr, I2C_WRITE, (void*)(&i2c));

    lcdPulseEnable(addr, data);
}

// Отправка полного байта (делится на 2 посылки по 4 бита)
void lcdSendByte(uint64_t addr, char value, char mode) {
  	lcdSend4Bits(addr, value & 0xF0, mode);
  	lcdSend4Bits(addr, (value << 4) & 0xF0, mode);
}

// Отправить команду (RS = 0)
void lcdCommand(uint64_t addr, char cmd) {
  	lcdSendByte(addr, cmd, 0);

  	if(cmd == 0x01 || cmd == 0x02) {
        // delay(2); // Команды очистки требуют больше времени
		for(int i = 0; i < 200000; ++i) {};
  	}
}

// Отправить символ/данные (RS = 1)
void lcdData(uint64_t addr, char data) {
  	lcdSendByte(addr, data, 0x01);
}

// Вывод строки
void lcdPrint(uint64_t addr, const char* str) {
  	while(*str) lcdData(addr, *str++);
}

// Установка курсора (колонка 0..19, строка 0..3)
void lcdSetCursor(uint64_t addr, char col, char row) {
  	int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
  	lcdCommand(addr, 0x80 | (col + row_offsets[row]));
}

/* AHT20 */
int aht20_init(uint64_t addr, uint64_t addrSlave) {
    struct I2cArg i2c = {.mode=MASTER,.addr=addrSlave};
  	ioctl(addr, I2C_SET_MODE, (void*)&i2c);
	// 40 ms
	for(int i = 0; i < 10000; ++i) {};

    uint8_t status = 0;
    i2c.buf=(void*)&status; i2c.lenData=1;
    ioctl(addr, I2C_READ, (void*)&i2c);

    // Проверяем бит калибровки
    if((status & 0x08) == 0) {
        uint8_t cmd[] = {0xBE, 0x08, 0x00};

        i2c.buf=(void*)&cmd; i2c.lenData=3;
        ioctl(addr, I2C_WRITE, (void*)&i2c);
		// 10 ms
		for(int i = 0; i < 10000; ++i) {};

        i2c.buf=(void*)&status; i2c.lenData=1;
        ioctl(addr, I2C_READ, (void*)&i2c);

        if((status & 0x08) == 0) return -5;
    }

    return 0;
}

int aht20_read(uint64_t addr, int32_t* temperature, uint32_t* humidity) {
    if(!temperature || !humidity) return -1;

    uint8_t cmd[] = {0xAC, 0x33, 0x00};
    struct I2cArg i2c = {.buf=cmd,.lenData=3};

    ioctl(addr, I2C_WRITE, (void*)&i2c);
	// 80 ms
	for(int i = 0; i < 100000; ++i) {};

    uint8_t data[6];
    // Читаем измерения
    i2c.buf=(void*)&data; i2c.lenData=6;
    ioctl(addr, I2C_READ, (void*)&i2c);

    // Бит 7: датчик ещё занят
    if(data[0] & 0x80) return -5;

    uint32_t rawHumidity = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (data[3] >> 4);
    uint32_t rawTemperature = ((uint32_t)(data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];

    // Результаты в сотых долях единицы:
    // temperature = сотые доли градуса C
    // humidity = сотые доли процента RH
    *humidity = (uint32_t)(((uint64_t)rawHumidity * 10000) / 1048576);
    *temperature = (int32_t)(((uint64_t)rawTemperature * 20000) / 1048576) - 5000;
	
    return 0;
}

/* BMP 280 */
static uint16_t bmpU16(uint8_t* data) {
    return (uint16_t)data[0] | ((uint16_t)data[1] << 8);
}

static int16_t bmpS16(uint8_t* data) {
    uint16_t value = bmpU16(data);

    return (int16_t)((value & 0x8000) ? (int32_t)value - 65536 : (int32_t)value);
}

int bmp280_init(uint64_t addr, uint64_t addrSlave, struct BMP280* bmp) {
    if(bmp == 0) return -1;

    struct I2cArg i2c = {.mode=MASTER,.addr=addrSlave};
    ioctl(DEVICE_I2C1, I2C_SET_MODE, (void*)&i2c);

    uint8_t reg = 0xD0;
    uint8_t id = 0;
    i2c.reg=reg; i2c.rData=(void*)&id; i2c.rLen=1;
    ioctl(addr, I2C_READ_REG, (void*)(&i2c));

    // Проверяем идентификатор
    if(id != 0x58) {
        printfUART(DEVICE_UART0, "BMP280 ID = %x\r\n", id);
        return -2;
    }

    // Читаем 24 байта калибровочных коэффициентов
    reg = 0x88;
    uint8_t data[24] = {0};

    i2c.reg = reg; i2c.rData = (void*)data; i2c.rLen = 24;
    ioctl(addr, I2C_READ_REG, (void*)(&i2c));

    bmp->T1 = bmpU16(&data[0]);
    bmp->T2 = bmpS16(&data[2]);
    bmp->T3 = bmpS16(&data[4]);

    bmp->P1 = bmpU16(&data[6]);
    bmp->P2 = bmpS16(&data[8]);
    bmp->P3 = bmpS16(&data[10]);
    bmp->P4 = bmpS16(&data[12]);
    bmp->P5 = bmpS16(&data[14]);
    bmp->P6 = bmpS16(&data[16]);
    bmp->P7 = bmpS16(&data[18]);
    bmp->P8 = bmpS16(&data[20]);
    bmp->P9 = bmpS16(&data[22]);

    if(bmp->P1 == 0) return -3;

    // Температура x1, давление x1, Normal Mode
    uint8_t config[] = {0xF4, 0x27};

    i2c.buf = (void*)config; i2c.lenData = 2;
    ioctl(addr, I2C_WRITE, (void*)(&i2c));

	/* 20 ms */
	for(int i = 0; i < 100000; ++i) {};

    return 0;
}

int bmp280_read(uint64_t addr, struct BMP280* bmp, int32_t* temperature, uint32_t* pressure) {
    if(!bmp || !temperature || !pressure) return -1;

    // Читаем 6 байтов начиная с 0xF7
    uint8_t reg = 0xF7;
    uint8_t data[6] = {0};
    struct I2cArg i2c = {.reg=reg,.rData=(void*)data,.rLen=6};
    
    ioctl(addr, I2C_READ_REG, (void*)(&i2c));

    int32_t adcP = ((int32_t)data[0] << 12) | ((int32_t)data[1] << 4) | ((int32_t)data[2] >> 4);

    int32_t adcT = ((int32_t)data[3] << 12) | ((int32_t)data[4] << 4) | ((int32_t)data[5] >> 4);

    if(adcT == 0x80000 || adcP == 0x80000) return -2;

    // Компенсация температуры
    int64_t var1, var2;

    var1 = (((adcT >> 3) - 2LL * bmp->T1) * bmp->T2) >> 11;

    int64_t x = (adcT >> 4) - bmp->T1;

    var2 = (((x * x) >> 12) * bmp->T3) >> 14;

    int32_t tFine = (int32_t)(var1 + var2);

    *temperature = (int32_t)(((int64_t)tFine * 5 + 128) >> 8);

    // Компенсация давления
    var1 = (int64_t)tFine - 128000;

    var2 = var1 * var1 * bmp->P6;
    var2 += var1 * bmp->P5 * 131072LL;
    var2 += (int64_t)bmp->P4 * (1LL << 35);

    var1 = ((var1 * var1 * bmp->P3) >> 8) + (var1 * bmp->P2 * 4096LL);

    var1 = (((1LL << 47) + var1) * bmp->P1) >> 33;

    if(var1 == 0) return -3;

    int64_t p = (((1048576LL - adcP) * (1LL << 31) - var2) * 3125LL) / var1;

    var1 = (bmp->P9 * (p >> 13) * (p >> 13)) >> 25;
    var2 = (bmp->P8 * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (int64_t)bmp->P7 * 16;

    if(p <= 0) return -4;

    // Из Q24.8 переводим в целые паскали
    *pressure = (uint32_t)(p / 256);

    return 0;
}