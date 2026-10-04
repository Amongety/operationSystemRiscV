.option norvc				/* Гарантия 4 байтного размера инструкций */

.section .text
.align 4				/* Гарантия адреса кратного 4 байтам */
.global user_trap_vector
user_trap_vector:
	csrrw t0, sscratch, t0

	sd ra, 8 * 0(t0)
	sd sp, 8 * 1(t0)
	sd gp, 8 * 2(t0)
	sd tp, 8 * 3(t0)
	sd t1, 8 * 5(t0)
	sd t2, 8 * 6(t0)
	sd fp, 8 * 7(t0)		/* s0 */
	sd s1, 8 * 8(t0)
	sd a0, 8 * 9(t0)
	sd a1, 8 * 10(t0)
	sd a2, 8 * 11(t0)
	sd a3, 8 * 12(t0)
	sd a4, 8 * 13(t0)
	sd a5, 8 * 14(t0)
	sd a6, 8 * 15(t0)
	sd a7, 8 * 16(t0)
	sd s2, 8 * 17(t0)
	sd s3, 8 * 18(t0)
	sd s4, 8 * 19(t0)
	sd s5, 8 * 20(t0)
	sd s6, 8 * 21(t0)
	sd s7, 8 * 22(t0)
	sd s8, 8 * 23(t0)
	sd s9, 8 * 24(t0)
	sd s10, 8 * 25(t0)
	sd s11, 8 * 26(t0)
	sd t3, 8 * 27(t0)
	sd t4, 8 * 28(t0)
	sd t5, 8 * 29(t0)
	sd t6, 8 * 30(t0)
	mv t1, t0
	csrr t0, sscratch
	sd t0, 8 * 4(t1)
	csrw sscratch, t1
	
	/* tf_csr_t */
	csrr t0, scause
	sd t0, 8 * 31(t1)
	csrr t0, stval
	sd t0, 8 * 32(t1)
	csrr t0, sepc
	sd t0, 8 * 33(t1)
	csrr t0, sstatus
	sd t0, 8 * 34(t1)
	csrr t0, satp
	sd t0, 8 * 35(t1)

	la t0, kernel_trap_vector
	csrw stvec, t0

	la sp, _stack

	mv a0, t1
	call handler_trap

	csrw sepc, a0

	la t0, user_trap_vector
	csrw stvec, t0

	csrr t0, sscratch

	ld ra, 8 * 0(t0)
	ld gp, 8 * 2(t0)
	ld tp, 8 * 3(t0)
	ld t1, 8 * 5(t0)
	ld t2, 8 * 6(t0)
	ld fp, 8 * 7(t0)		/* s0 */
	ld s1, 8 * 8(t0)
	ld a0, 8 * 9(t0)
	ld a1, 8 * 10(t0)
	ld a2, 8 * 11(t0)
	ld a3, 8 * 12(t0)
	ld a4, 8 * 13(t0)
	ld a5, 8 * 14(t0)
	ld a6, 8 * 15(t0)
	ld a7, 8 * 16(t0)
	ld s2, 8 * 17(t0)
	ld s3, 8 * 18(t0)
	ld s4, 8 * 19(t0)
	ld s5, 8 * 20(t0)
	ld s6, 8 * 21(t0)
	ld s7, 8 * 22(t0)
	ld s8, 8 * 23(t0)
	ld s9, 8 * 24(t0)
	ld s10, 8 * 25(t0)
	ld s11, 8 * 26(t0)
	ld t3, 8 * 27(t0)
	ld t4, 8 * 28(t0)
	ld t5, 8 * 29(t0)
	ld t6, 8 * 30(t0)
	ld sp, 8 * 1(t0)
	ld t0, 8 * 4(t0)

	sret

.section .text
.align 4				/* Гарантия адреса кратного 4 байтам */
.global kernel_trap_vector
kernel_trap_vector:
	addi sp, sp, -8 * 36
	sd t0, 8 * 4(sp)

	addi t0, sp, 8 * 36
	sd t0, 8 * 1(sp)
	
	sd ra, 8 * 0(sp)
	sd gp, 8 * 2(sp)
	sd tp, 8 * 3(sp)
	sd t1, 8 * 5(sp)
	sd t2, 8 * 6(sp)
	sd fp, 8 * 7(sp)		/* s0 */
	sd s1, 8 * 8(sp)
	sd a0, 8 * 9(sp)
	sd a1, 8 * 10(sp)
	sd a2, 8 * 11(sp)
	sd a3, 8 * 12(sp)
	sd a4, 8 * 13(sp)
	sd a5, 8 * 14(sp)
	sd a6, 8 * 15(sp)
	sd a7, 8 * 16(sp)
	sd s2, 8 * 17(sp)
	sd s3, 8 * 18(sp)
	sd s4, 8 * 19(sp)
	sd s5, 8 * 20(sp)
	sd s6, 8 * 21(sp)
	sd s7, 8 * 22(sp)
	sd s8, 8 * 23(sp)
	sd s9, 8 * 24(sp)
	sd s10, 8 * 25(sp)
	sd s11, 8 * 26(sp)
	sd t3, 8 * 27(sp)
	sd t4, 8 * 28(sp)
	sd t5, 8 * 29(sp)
	sd t6, 8 * 30(sp)
	
	/* tf_csr_t */
	csrr a0, scause
	sd a0, 8 * 31(sp)
	csrr a0, stval
	sd a0, 8 * 32(sp)
	csrr a0, sepc
	sd a0, 8 * 33(sp)
	csrr a0, sstatus
	sd a0, 8 * 34(sp)
	csrr a0, satp
	sd a0, 8 * 35(sp)

	mv a0, sp
	call handler_trap

	csrw sepc, a0

	ld ra, 8 * 0(sp)
	ld gp, 8 * 2(sp)
	ld tp, 8 * 3(sp)
	ld t0, 8 * 4(sp)
	ld t1, 8 * 5(sp)
	ld t2, 8 * 6(sp)
	ld fp, 8 * 7(sp)		/* s0 */
	ld s1, 8 * 8(sp)
	ld a0, 8 * 9(sp)
	ld a1, 8 * 10(sp)
	ld a2, 8 * 11(sp)
	ld a3, 8 * 12(sp)
	ld a4, 8 * 13(sp)
	ld a5, 8 * 14(sp)
	ld a6, 8 * 15(sp)
	ld a7, 8 * 16(sp)
	ld s2, 8 * 17(sp)
	ld s3, 8 * 18(sp)
	ld s4, 8 * 19(sp)
	ld s5, 8 * 20(sp)
	ld s6, 8 * 21(sp)
	ld s7, 8 * 22(sp)
	ld s8, 8 * 23(sp)
	ld s9, 8 * 24(sp)
	ld s10, 8 * 25(sp)
	ld s11, 8 * 26(sp)
	ld t3, 8 * 27(sp)
	ld t4, 8 * 28(sp)
	ld t5, 8 * 29(sp)
	ld t6, 8 * 30(sp)
	ld sp, 8 * 1(sp)

	sret