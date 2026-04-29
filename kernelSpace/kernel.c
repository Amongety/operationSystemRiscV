#include "../include/kernelSpace/config.h"
#include "../include/kernelSpace/arch/riscV/kernel_init.h"
#include "../include/kernelSpace/libsbi/te.h"

void main(struct dtbPlatform dtb) 
{
	globalDTB = dtb;
	
	uint64_t* root_page_table = (uint64_t*)alloc_pages(1);

	init_virtual_memory(root_page_table);
	
	init_uart(queryMode, globalDTB.uart[0].addr);
	if(!init_sdmmc(globalDTB.sd.addr)) PANIC("Error init SD-CARD");	

	console_printf("Text %x %x\r\n", _text_start, _text_end);
	console_printf("Rodata %x %x\r\n", _rodata_start, _rodata_end);
	console_printf("Data %x %x\r\n", _data_start, _data_end);
	console_printf("Bss %x %x\r\n", _bss_start, _bss_end);
	console_printf("Stack %x %x\r\n", _stack - 128 * 1024, _stack);
	console_printf("FreeRam %x %x\r\n", _free_ram_start, _free_ram_end);

	console_printf("Hello debug SBI!\r\n");

	console_printf("UART. Addr: %x. Size: %x\r\n", globalDTB.uart[0].addr, globalDTB.uart[0].size);
	console_printf("SD CARD. Addr: %x. Size: %x\r\n", globalDTB.sd.addr, globalDTB.sd.size);

	writeSDMMC(globalDTB.sd.addr, NONDMA, 247808, (void*)"PERSONS REPRESENTED\r\n"\
														"Escalus, Prince of Verona.\r\n"\
														"Paris, a young Nobleman, kinsman to the Prince.\r\n"\
														"Montague,}Heads of two Houses at variance with each other.\r\n"\
														"Capulet, }\r\n"\
														"An Old Man, Uncle to Capulet.\r\n"\
														"Romeo, Son to Montague.\r\n"\
														"Mercutio, Kinsman to the Prince, and Friend to Romeo.\r\n"\
														"Benvolio, Nephew to Montague, and Friend to Romeo.\r\n"\
														"Tybalt, Nephew to Lady Capulet.\r\n"\
														"Friar Lawrence, a Franciscan.\r\n"\
														"Friar John, of the same Order.\r\n"\
														"Balthasar, Servant to Romeo.\r\n"\
														"Sampson, Servant to Capulet.\r\n"\
														"Gregory, Servant to Capulet.\r\n"\
														"Peter, Servant to Juliet's Nurse.\r\n"\
														"Abraham, Servant to Montague.\r\n"\
														"An Apothecary.\r\n"\
														"Three Musicians.\r\n"\
														"Chorus.\r\n"\
														"Page to Paris; another Page.\r\n"\
														"An Officer.\r\n"\
														"Lady Montague, Wife to Montague.\r\n"\
														"Lady Capulet, Wife to Capulet.\r\n"\
														"Juliet, Daughter to Capulet.\r\n"\
														"Nurse to Juliet.\r\n"\
														"Citizens of Verona; several Men and Women, relations to both\r\n"\
														"houses; Maskers, Guards, Watchmen, and Attendants.\r\n"\
														"SCENE. — During the greater part of the Play in Verona; once, in the Fifth Act, at Mantua.\r\n"\
														"THE PROLOGUE\r\n"\
														, 1008);
	writeSDMMC(globalDTB.sd.addr, NONDMA, 247810, (void*)"Hello SD CARD!?\r\n", 17);
	writeSDMMC(globalDTB.sd.addr, NONDMA, 247811, (void*)"Hello SD CARD!?\r\n", 17);
	writeSDMMC(globalDTB.sd.addr, NONDMA, 62333951, (void*)"Hello SD CARD!?\r\n", 17);
	uint8_t t[1008];
	readSDMMC(globalDTB.sd.addr, NONDMA, 247808, (void*)t, 1008);
	console_printf("OUT: %s\r\n", t);

	/*
	create_process(0x80800000); // userSpace/user_test.c	 void main(void)
	create_process(0x80800000);
	create_process(0x80800000); 
	sbi_set_timer(1);
	*/

	while(1) {
		asm("wfi");
	}
}

