.section .text

/* Read csr */
.global read_csr_scause
read_csr_scause:
	csrr a0, scause
	ret

.global read_csr_stval
read_csr_stval:
	csrr a0, stval
	ret

.global read_csr_sepc
read_csr_sepc:
	csrr a0, sepc
	ret

/* Write csr */
