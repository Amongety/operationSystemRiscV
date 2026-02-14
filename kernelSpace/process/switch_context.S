
.section .text
.global switchProc
switchProc:
	csrw sscratch, a0

	li t1, 0x2 | 0x20 | 0x100
	csrs sstatus, t1 			/* csrs - установка битов (ИЛИ) */

	csrw sepc, a1
	csrw satp, a2
	sfence.vma
	li t1, 0x222
	csrs sie, t1
	la t2, asm_trap_vector
	csrw stvec, t2

	li t1, 0x100
	csrc sstatus, t1 			/* csrc - сброс бита (И) */

	ld ra, 8 * 0(a0)
	ld sp, 8 * 1(a0)
	ld gp, 8 * 2(a0)
	ld tp, 8 * 3(a0)
	ld t0, 8 * 4(a0)
	ld t1, 8 * 5(a0)
	ld t2, 8 * 6(a0)
	ld fp, 8 * 7(a0)		/* s0 */
	ld s1, 8 * 8(a0)
	ld a1, 8 * 10(a0)
	ld a2, 8 * 11(a0)
	ld a3, 8 * 12(a0)
	ld a4, 8 * 13(a0)
	ld a5, 8 * 14(a0)
	ld a6, 8 * 15(a0)
	ld a7, 8 * 16(a0)
	ld s2, 8 * 17(a0)
	ld s3, 8 * 18(a0)
	ld s4, 8 * 19(a0)
	ld s5, 8 * 20(a0)
	ld s6, 8 * 21(a0)
	ld s7, 8 * 22(a0)
	ld s8, 8 * 23(a0)
	ld s9, 8 * 24(a0)
	ld s10, 8 * 25(a0)
	ld s11, 8 * 26(a0)
	ld t3, 8 * 27(a0)
	ld t4, 8 * 28(a0)
	ld t5, 8 * 29(a0)
	ld t6, 8 * 30(a0)
	
	li a0, 0

	sret	
