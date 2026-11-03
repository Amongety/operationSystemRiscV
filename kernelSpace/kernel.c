#include "../include/kernelSpace/config.h"
#include "../include/kernelSpace/arch/riscV/kernel_init.h"
#include "../include/kernelSpace/libsbi/te.h"
#include "../include/kernelSpace/filesystem/fs.h"
#include "../include/kernelSpace/drivers/gpio/gpio.h"
#include "../include/kernelSpace/drivers/i2c/i2c.h"


// Импульс на выводе Enable (EN)
void lcdPulseEnable(char data) {
	char t = data | 0x04;
  writeI2c(globalDTB.i2c[1].addr, &t, 1);
//   delayMicroseconds(1);
	t = data & ~0x04;
  writeI2c(globalDTB.i2c[1].addr, &t, 1);
//   delayMicroseconds(50);
}

// Отправка 4 бит данных/команды
void lcdSend4Bits(char nibble, char mode) {
  char data = (nibble & 0xF0) | mode | 0x08;
  writeI2c(globalDTB.i2c[1].addr, &data, 1);
  lcdPulseEnable(data);
}

// Отправка полного байта (делится на 2 посылки по 4 бита)
void lcdSendByte(char value, char mode) {
  lcdSend4Bits(value & 0xF0, mode);
  lcdSend4Bits((value << 4) & 0xF0, mode);
}

// Отправить команду (RS = 0)
void lcdCommand(char cmd) {
  lcdSendByte(cmd, 0);
  if (cmd == 0x01 || cmd == 0x02) {
	for(int i = 0; i < 200000; ++i) {};
    // delay(2); // Команды очистки требуют больше времени
  }
}

// Отправить символ/данные (RS = 1)
void lcdData(char data) {
  lcdSendByte(data, 0x01);
}

// Вывод строки
void lcdPrint(const char* str) {
  while (*str) {
    lcdData(*str++);
  }
}

// Установка курсора (колонка 0..19, строка 0..3)
void lcdSetCursor(char col, char row) {
  int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
  lcdCommand(0x80 | (col + row_offsets[row]));
}

#define PINMUX_I2C1_SDA 0x03001134UL
#define PINMUX_I2C1_SCL 0x03001138UL

void init_pinmux_i2c1(void) {
    *(volatile uint32_t *)PINMUX_I2C1_SDA = 6;
    *(volatile uint32_t *)PINMUX_I2C1_SCL = 6;
}



void main(struct DtbPlatform DTB) 
{
	globalDTB = DTB;
	
	uint64_t* root_page_table = (uint64_t*)alloc_pages(1);

	init_virtual_memory(root_page_table);
	
	init_uart(queryMode, globalDTB.uart[0].addr);
	if(!init_sdmmc(globalDTB.sd.addr)) PANIC("Error init SD-CARD");
	
	fsInitMinix3();

	console_printf(globalDTB.uart[0].addr, "Text %x %x\r\n", _text_start, _text_end);
	console_printf(globalDTB.uart[0].addr, "Rodata %x %x\r\n", _rodata_start, _rodata_end);
	console_printf(globalDTB.uart[0].addr, "Data %x %x\r\n", _data_start, _data_end);
	console_printf(globalDTB.uart[0].addr, "Bss %x %x\r\n", _bss_start, _bss_end);
	console_printf(globalDTB.uart[0].addr, "Stack %x %x\r\n", _stack - 128 * 1024, _stack);
	console_printf(globalDTB.uart[0].addr, "FreeRam %x %x\r\n", _free_ram_start, _free_ram_end);

	console_printf(globalDTB.uart[0].addr, "Hello debug SBI!\r\n");

	for(int i = 0; i < UART_MAX; ++i) console_printf(globalDTB.uart[0].addr, "UART%d. Addr: %x. Size: %x\r\n", i, globalDTB.uart[i].addr, globalDTB.uart[i].size);
	console_printf(globalDTB.uart[0].addr, "SD CARD. Addr: %x. Size: %x\r\n", globalDTB.sd.addr, globalDTB.sd.size);
	for(int i = 0; i < GPIO_MAX; ++i) console_printf(globalDTB.uart[0].addr, "GPIO%d. Addr: %x. Size: %x\r\n", i, globalDTB.gpio[i].addr, globalDTB.gpio[i].size);
	for(int i = 0; i < I2C_MAX; ++i) console_printf(globalDTB.uart[0].addr, "I2C%d. Addr: %x. Size: %x\r\n", i, globalDTB.i2c[i].addr, globalDTB.i2c[i].size);

	// create_process("/user.elf");
	// create_process("/user.elf");
	// sbi_set_timer(1);

	/*long fd = mkdirMinix3("/trt", 0755);
	long fd2 = openMinix3("/trt/testFile", 0755);
	console_printf(globalDTB.uart[0].addr, "Directory = %d\r\n", fd);
	console_printf(globalDTB.uart[0].addr, "File = %d\r\n", fd2);

	uint8_t bufTest[1035];

	console_printf(globalDTB.uart[0].addr, "Count write = %d\r\n", writeInodeMinix3(fd2, (uint8_t*)"XYZZY123", 8, 0));
	console_printf(globalDTB.uart[0].addr, "Count read = %d\r\n", readInodeMinix3(fd2, bufTest, 8, 0));

	for(int i = 0; i < 8; ++i) {
		if(bufTest[i] == '\n') console_printf("\r");
		console_printf(globalDTB.uart[0].addr, "%c", bufTest[i]);
	}
	console_printf(globalDTB.uart[0].addr, "\r\n");

	unlinkMinix3("/trt/testFile");
	rmdirMinix3("/trt");*/

	/*
	create_process(0x80800000); // userSpace/user_test.c	 void main(void)
	create_process(0x80800000);
	create_process(0x80800000); 
	sbi_set_timer(1);
	*/

	console_printf(globalDTB.uart[0].addr, "\r\nDONE INIT KERNEL\r\n");

	init_pinmux_i2c1();

	init_i2c(globalDTB.i2c[1].addr, MASTER, 0x27); // 0x3F 0x27

	lcdSend4Bits(0x30, 0);
	// delay(5);
	for(int i = 0; i < 500000; ++i) {};
	lcdSend4Bits(0x30, 0);
	// delayMicroseconds(150);
	for(int i = 0; i < 150000; ++i) {};
	lcdSend4Bits(0x30, 0);
	lcdSend4Bits(0x20, 0); // Переход в 4-битный режим

	lcdCommand(0x28); // 2 строки (для логики дисплея), 5x8 матрица
	lcdCommand(0x0C); // Дисплей включен, курсор выключен
	lcdCommand(0x06); // Автоинкремент курсора
	lcdCommand(0x01); // Очистка дисплея
	// delay(2);
	for(int i = 0; i < 200000; ++i) {};

	// Вывод текста по строкам для 2004 (20 символов, 4 строки)
	lcdSetCursor(0, 0);
	lcdPrint("Hello, LCD 2004!");
	lcdSetCursor(0, 1);
	lcdPrint("I2C without lib.");
	lcdSetCursor(0, 2);
	lcdPrint("Line 3: OK");
	lcdSetCursor(0, 3);
	lcdPrint("Line 4: Ready");
	for(int i = 0; i < 5000000; ++i) {};

	lcdCommand(0x01);

	/*init_GPIO(globalDTB.gpio[0].addr, 29, GPIO_OUTPUT);
	while(true) {
		setGpio(globalDTB.gpio[0].addr, 29, 1);
		console_printf(globalDTB.uart[0].addr, "Set 1\r\n");
		for(volatile int i = 0; i < 5000000; ++i) asm("nop");
		console_printf(globalDTB.uart[0].addr, "Set 0\r\n");
		setGpio(globalDTB.gpio[0].addr, 29, 0);
		for(volatile int i = 0; i < 5000000; ++i) asm("nop");
	}*/

	while(1) {
		asm("wfi");
	}
}

/*
PERSONS REPRESENTED
Escalus, Prince of Verona.
Paris, a young Nobleman, kinsman to the Prince.
Montague,}Heads of two Houses at variance with each other.
Capulet, }
An Old Man, Uncle to Capulet.
Romeo, Son to Montague.
Mercutio, Kinsman to the Prince, and Friend to Romeo.
Benvolio, Nephew to Montague, and Friend to Romeo.
Tybalt, Nephew to Lady Capulet.
Friar Lawrence, a Franciscan.
Friar John, of the same Order.
Balthasar, Servant to Romeo.
Sampson, Servant to Capulet.
Gregory, Servant to Capulet.
Peter, Servant to Juliet's Nurse.
Abraham, Servant to Montague.
An Apothecary.
Three Musicians.
Chorus.
Page to Paris; another Page.
An Officer.
Lady Montague, Wife to Montague.
Lady Capulet, Wife to Capulet.
Juliet, Daughter to Capulet.
Nurse to Juliet.
Citizens of Verona; several Men and Women, relations to both
houses; Maskers, Guards, Watchmen, and Attendants.
SCENE. — During the greater part of the Play in Verona; once, in the Fifth Act, at Mantua.
THE PROLOGUE

*/