.section .text
.global kernel_init
kernel_init:
	/* Загрузка стека */
	la sp, _stack
	
	/* Загрузка программного обработчика */
	la t0, asm_trap_vector
	csrw stvec, t0

	ret

