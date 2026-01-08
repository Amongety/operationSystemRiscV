.section .text
.global boot
boot:
/* sbi_debug_console_write */
	li a7, 0x4442434E
	li a6, 0x0
	li a0, 12
	la a1, debug_string
	li a2, 0
	ecall

/* sbi_debug_console_write_byte */
	li a7, 0x4442434E
	li a6, 0x2
	la a0, 'a'
	ecall

	li t0, 0x80400000
	jr t0

wfi_loop:
	wfi
	j wfi_loop

debug_string:
	.string "Hello\n"
