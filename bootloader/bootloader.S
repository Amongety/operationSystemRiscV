.section .text
.global boot
boot:
	la sp, _bootStack
	
	/* Добавить загрузку программы из диска в память */
	
	li t0, 0x80400000
	jr t0			/* прыжок в ядро */

wfi_loop:
	wfi
	j wfi_loop
