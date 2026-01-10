.section .text
.global boot
boot:
	/* Исправить инициализацию стека !*/
	la sp, 0x80600000

	/* Добавить загрузку программы из диска в память */

	li t0, 0x80400000
	jr t0

wfi_loop:
	wfi
	j wfi_loop
