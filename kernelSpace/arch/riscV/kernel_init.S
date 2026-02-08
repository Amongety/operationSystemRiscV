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

	/* Загрузка программного обработчика */
	la t0, asm_trap_vector
	csrw stvec, t0

	ret

/* Инициализация виртульной памяти */
.section .text
.global init_virtual_memory
init_virtual_memory:
	addi sp, sp, -16
	sw a0, 8(sp)
	sw ra, 12(sp)
	
	lw a0, 8(sp)
	la a1, _free_ram_start
	la a2, _free_ram_end
	li a3, 0x6
	call id_map_page_range

	lw a0, 8(sp)
	la a1, _text_start
	la a2, _text_end
	li a3, 0xA
	call id_map_page_range

	lw a0, 8(sp)
	la a1, _rodata_start
	la a2, _rodata_end
	li a3, 0xA
	call id_map_page_range

	lw a0, 8(sp)
	la a1, _data_start
	la a2, _data_end
	li a3, 0x6
	call id_map_page_range

	lw a0, 8(sp)
	la a1, _bss_start
	la a2, _bss_end
	li a3, 0x6
	call id_map_page_range

	lw a0, 8(sp)
	la a1, _stack - 128 * 1024
	la a2, _stack
	li a3, 0x6
	call id_map_page_range

	lw a0, 8(sp)
	mv t0, a0
	srli t0, t0, 12
	li t1, 0x80000000
	or t0, t0, t1
	csrw satp, t0
	sfence.vma

	lw ra, 12(sp)
	addi sp, sp, 16

	ret
