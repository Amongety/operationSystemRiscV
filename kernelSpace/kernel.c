#include "../include/kernelSpace/config.h"
#include "../include/kernelSpace/arch/riscV/kernel_init.h"
#include "../include/kernelSpace/libsbi/te.h"
#include "../include/kernelSpace/filesystem/fs.h"

void main(struct DtbPlatform DTB) 
{
	globalDTB = DTB;
	
	uint64_t* root_page_table = (uint64_t*)alloc_pages(1);

	init_virtual_memory(root_page_table);
	
	init_uart(queryMode, globalDTB.uart[0].addr);
	if(!init_sdmmc(globalDTB.sd.addr)) PANIC("Error init SD-CARD");	
	
	fsInitMinix3();

	console_printf("Text %x %x\r\n", _text_start, _text_end);
	console_printf("Rodata %x %x\r\n", _rodata_start, _rodata_end);
	console_printf("Data %x %x\r\n", _data_start, _data_end);
	console_printf("Bss %x %x\r\n", _bss_start, _bss_end);
	console_printf("Stack %x %x\r\n", _stack - 128 * 1024, _stack);
	console_printf("FreeRam %x %x\r\n", _free_ram_start, _free_ram_end);

	console_printf("Hello debug SBI!\r\n");

	console_printf("UART. Addr: %x. Size: %x\r\n", globalDTB.uart[0].addr, globalDTB.uart[0].size);
	console_printf("SD CARD. Addr: %x. Size: %x\r\n", globalDTB.sd.addr, globalDTB.sd.size);

	long fd = mkdirMinix3("/trt", 0755);
	long fd2 = openMinix3("/trt/testFile", 0755);
	console_printf("Directory = %d\r\n", fd);
	console_printf("File = %d\r\n", fd2);

	uint8_t bufTest[1035];

	console_printf("Count write = %d\r\n", writeInodeMinix3(fd2, (uint8_t*)"XYZZY123", 8, 0));
	console_printf("Count read = %d\r\n", readInodeMinix3(fd2, bufTest, 8, 0));

	for(int i = 0; i < 8; ++i) {
		if(bufTest[i] == '\n') console_printf("\r");
		console_printf("%c", bufTest[i]);
	}
	console_printf("\r\n");

	unlinkMinix3("/trt/testFile");
	rmdirMinix3("/trt");

	/*
	create_process(0x80800000); // userSpace/user_test.c	 void main(void)
	create_process(0x80800000);
	create_process(0x80800000); 
	sbi_set_timer(1);
	*/

	console_printf("\r\nDONE INIT KERNEL\r\n");

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