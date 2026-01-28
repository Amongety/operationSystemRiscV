/* Timer Extension */

.section .text

/* 1s: a0 = 10000000 */
.global sbi_debug_set_timer
sbi_debug_set_timer:
	rdtime t0
	add a0, a0, t0

	li a6, 0x0
	li a7, 0x54494D45
	ecall

	li t0, 0x20
	csrs sie, t0

	ret

