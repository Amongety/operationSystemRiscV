.section .text
.global _start
_start:
	call main

	/* exit */
	li a7, 0x5d
	ecall

