/* Timer Extension */

.section .text

.global sbi_debug_set_timer
sbi_debug_set_timer:
	li a7, 0x54494D45
	li a6, 0x0
	ecall
	ret

