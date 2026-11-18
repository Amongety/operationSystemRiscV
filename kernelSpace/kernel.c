#include "../include/kernelSpace/config.h"
#include "../include/kernelSpace/arch/riscV/kernel_init.h"

#define PINMUX_GPIO_A16 0x03001040
#define PINMUX_GPIO_A17 0x03001044
#define PINMUX_GPIO_A18 0x03001068
#define PINMUX_GPIO_A19 0x03001064
#define PINMUX_GPIO_A20 0x0300106C
#define PINMUX_GPIO_A28 0x03001070

int init_pinmux_gpioA(uint8_t pin) {
    uint64_t addr;

    switch(pin) {
        case 16: addr = PINMUX_GPIO_A16; break;
        case 17: addr = PINMUX_GPIO_A17; break;
        case 18: addr = PINMUX_GPIO_A18; break;
        case 19: addr = PINMUX_GPIO_A19; break;
        case 20: addr = PINMUX_GPIO_A20; break;
        case 28: addr = PINMUX_GPIO_A28; break;
        default: return -1;
    }

    *(volatile uint32_t*)addr = 3;
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
	
	// init_uart(&globalDTB.uart[0], queryMode);		// СРОЧНЫЙ ФИКС!
	if(!init_sdmmc(&globalDTB.sd)) PANIC("Error init SD-CARD");
	
	fsInitMinix3();

	console_printf(&globalDTB.uart[0], "Text %x %x\r\n", _text_start, _text_end);
	console_printf(&globalDTB.uart[0], "Rodata %x %x\r\n", _rodata_start, _rodata_end);
	console_printf(&globalDTB.uart[0], "Data %x %x\r\n", _data_start, _data_end);
	console_printf(&globalDTB.uart[0], "Bss %x %x\r\n", _bss_start, _bss_end);
	console_printf(&globalDTB.uart[0], "Stack %x %x\r\n", _stack - 128 * 1024, _stack);
	console_printf(&globalDTB.uart[0], "FreeRam %x %x\r\n", _free_ram_start, _free_ram_end);

	console_printf(&globalDTB.uart[0], "Hello debug SBI!\r\n");

	for(int i = 0; i < UART_MAX; ++i) console_printf(&globalDTB.uart[0], "UART%d. Addr: %x. Size: %x\r\n", i, globalDTB.uart[i].addr, globalDTB.uart[i].size);
	console_printf(&globalDTB.uart[0], "SD CARD. Addr: %x. Size: %x\r\n", globalDTB.sd.addr, globalDTB.sd.size);
	for(int i = 0; i < GPIO_MAX; ++i) console_printf(&globalDTB.uart[0], "GPIO%d. Addr: %x. Size: %x\r\n", i, globalDTB.gpio[i].addr, globalDTB.gpio[i].size);
	for(int i = 0; i < I2C_MAX; ++i) console_printf(&globalDTB.uart[0], "I2C%d. Addr: %x. Size: %x\r\n", i, globalDTB.i2c[i].addr, globalDTB.i2c[i].size);

	init_pinmux_gpioA(28);
	init_pinmux_i2c1();

	// create_process("/user.elf");
	// create_process("/user.elf");
	// sbi_set_timer(1);

	/*long fd = mkdirMinix3("/trt", 0755);
	long fd2 = openMinix3("/trt/testFile", 0755);
	console_printf(&globalDTB.uart[0], "Directory = %d\r\n", fd);
	console_printf(&globalDTB.uart[0], "File = %d\r\n", fd2);

	uint8_t bufTest[1035];

	console_printf(&globalDTB.uart[0], "Count write = %d\r\n", writeInodeMinix3(fd2, (uint8_t*)"XYZZY123", 8, 0));
	console_printf(&globalDTB.uart[0], "Count read = %d\r\n", readInodeMinix3(fd2, bufTest, 8, 0));

	for(int i = 0; i < 8; ++i) {
		if(bufTest[i] == '\n') console_printf(&globalDTB.uart[0], "\r");
		console_printf(&globalDTB.uart[0], "%c", bufTest[i]);
	}
	console_printf(&globalDTB.uart[0], "\r\n");

	unlinkMinix3("/trt/testFile");
	rmdirMinix3("/trt");*/

	console_printf(&globalDTB.uart[0], "\r\nDONE INIT KERNEL\r\n");

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