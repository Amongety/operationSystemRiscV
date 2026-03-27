.section .text
.global boot
boot:
	la sp, _bootStack
	mv fp, sp

	la gp, _bootStartData

	addi sp, sp, -16
	sd a0, 8 * 0(sp)
	sd a1, 8 * 1(sp)

	/* Добавить загрузку программы из диска в память */
	
	ld a0, 8 * 1(sp) 
	call init_dtb
	beq a0, zero, wfi_loop

	addi sp, sp, 16
	
	la a0, dtbPlt 
	li t0, 0x80400000
	jr t0			/* прыжок в ядро */

wfi_loop:
	wfi
	j wfi_loop
