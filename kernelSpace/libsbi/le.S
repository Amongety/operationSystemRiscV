/* Legacy Extensions */

.section .text

/* 1s: a0 = 10000000 */
.global sbi_set_timer
sbi_set_timer:
	rdtime t0
	add a0, a0, t0

	li a6, 0x0
	li a7, 0x0
	ecall

	li t0, 0x20
	csrs sie, t0

	ret

.global sbi_console_putchar
sbi_console_putchar:
	li a7, 0x01
	li a6, 0x0
	ecall
	ret

.global sbi_console_getchar
sbi_console_getchar:
	li a7, 0x02
	li a6, 0x0
	ecall
	ret
