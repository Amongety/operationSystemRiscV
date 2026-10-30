#include "../../../include/kernelSpace/drivers/mmio.h"

.section .text
.global kernel_init
kernel_init:
	/* Чтобы поймать в регисте sie STIE */
	li t1, 0x2 | 0x20 | 0x100
	csrs sstatus, t1

	/* Загрузка стека */
	la sp, _stack
	mv fp, sp
	csrw sscratch, sp

	la gp, _data_start

	addi sp, sp, -16
	sd ra, 0(sp)
	sd a0, 8(sp)

	call clear_bss

	/* Загрузка программного обработчика */
	la t0, kernel_trap_vector
	csrw stvec, t0

	ld ra, 0(sp)
	ld a0, 8(sp)
	addi sp, sp, 16

	call main
	ret

/* Инициализация виртульной памяти */
.section .text
.global init_virtual_memory
init_virtual_memory:
	addi sp, sp, -16
	sd a0, 0(sp)
	sd ra, 8(sp)
	
	ld a0, 0(sp)
	la a1, _free_ram_start
	la a2, _free_ram_end
	li a3, 0x6 | 0xC0
	call id_map_page_range

	ld a0, 0(sp)
	la a1, _text_start
	la a2, _text_end
	li a3, 0xA | 0xC0
	call id_map_page_range

	ld a0, 0(sp)
	la a1, _rodata_start
	la a2, _rodata_end
	li a3, 0xA | 0xC0
	call id_map_page_range

	ld a0, 0(sp)
	la a1, _data_start
	la a2, _data_end
	li a3, 0x6 | 0xC0
	call id_map_page_range

	ld a0, 0(sp)
	la a1, _bss_start
	la a2, _bss_end
	li a3, 0x6 | 0xC0
	call id_map_page_range

	ld a0, 0(sp)
	la a1, _stack - 128 * 1024
	la a2, _stack
	li a3, 0x6 | 0xC0
	call id_map_page_range

	/* UART */
    ld a0, 0(sp)
    la a1, globalDTB
    li a2, 5
    call map_dtb_group

    /* SD CARD */
    ld a0, 0(sp)
    la a1, globalDTB
    addi a1, a1, 80
    li a2, 1
    call map_dtb_group

    /* GPIO */
    ld a0, 0(sp)
    la a1, globalDTB
    addi a1, a1, 96
    li a2, 5
    call map_dtb_group

    /* I2C */
    ld a0, 0(sp)
    la a1, globalDTB
    addi a1, a1, 176
    li a2, 5
    call map_dtb_group
	
	ld a0, 0(sp)
	mv t0, a0
	srli t0, t0, 12
	li t1, 0x8000000000000000
	or t0, t0, t1
	csrw satp, t0
	sfence.vma
	fence.i
	
	ld ra, 8(sp)
	addi sp, sp, 16

	ret

/* Парсер для перефирии >= 1 */
map_dtb_group:
    addi sp, sp, -32
    sd ra, 24(sp)
    sd s0, 16(sp)
    sd s1, 8(sp)
    sd s2, 0(sp)

    mv s0, a0				/* root_page_table */
    mv s1, a1				/* указатель на структуру dtb */
    mv s2, a2				/* кол-во устройств */

1:
    beqz s2, 3f

    ld a1, 0(s1)			/* .addr */
    ld a2, 8(s1)			/* .size */

	/* Защита от нулевого адреса */
	beqz a2, 2f

    add a2, a1, a2			/* addr_end = addr_start + size */
    mv a0, s0
    li a3, 0x6 | 0xC0

    call id_map_page_range

/* Цикл кол-во интерфейса */
2:
    addi s1, s1, 16			/* Следующая структура */
    addi s2, s2, -1
    j 1b

3:
    ld s2, 0(sp)
    ld s1, 8(sp)
    ld s0, 16(sp)
    ld ra, 24(sp)
    addi sp, sp, 32
    ret