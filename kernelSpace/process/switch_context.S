
.section .text
.global switchProc
switchProc:
	csrw sscratch, a0

	li t1, (1 << 5) | (1 << 8)
	csrs sstatus, t1 			/* csrs - установка битов (ИЛИ) */

	csrw sepc, a1
	csrw satp, a2
	sfence.vma
	li t1, 0x222
	csrw sie, t1
	la t2, asm_trap_vector
	csrw stvec, t2

	li t1, 1 << 8
	csrc sstatus, t1 			/* csrc - сброс бита (И) */

	lw ra, 4 * 0(a0)
	lw sp, 4 * 1(a0)
	lw gp, 4 * 2(a0)
	lw tp, 4 * 3(a0)
	lw t0, 4 * 4(a0)
	lw t1, 4 * 5(a0)
	lw t2, 4 * 6(a0)
	lw fp, 4 * 7(a0)		/* s0 */
	lw s1, 4 * 8(a0)
	lw a1, 4 * 10(a0)
	lw a2, 4 * 11(a0)
	lw a3, 4 * 12(a0)
	lw a4, 4 * 13(a0)
	lw a5, 4 * 14(a0)
	lw a6, 4 * 15(a0)
	lw a7, 4 * 16(a0)
	lw s2, 4 * 17(a0)
	lw s3, 4 * 18(a0)
	lw s4, 4 * 19(a0)
	lw s5, 4 * 20(a0)
	lw s6, 4 * 21(a0)
	lw s7, 4 * 22(a0)
	lw s8, 4 * 23(a0)
	lw s9, 4 * 24(a0)
	lw s10, 4 * 25(a0)
	lw s11, 4 * 26(a0)
	lw t3, 4 * 27(a0)
	lw t4, 4 * 28(a0)
	lw t5, 4 * 29(a0)
	lw t6, 4 * 30(a0)
	li a0, 0

	sret	
