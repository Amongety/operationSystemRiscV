/* Debug Console Extension */

.section .text

.global sbi_debug_console_write
sbi_debug_console_write:
	li a7, 0x4442434E
	li a6, 0x0
	ecall
	ret

.global sbi_debug_console_read
sbi_debug_console_read:
	li a7, 0x4442434E
	li a6, 0x1
	ecall
	ret

.global sbi_debug_console_write_byte
sbi_debug_console_write_byte:
	li a7, 0x4442434E
	li a6, 0x2
	ecall
	ret

