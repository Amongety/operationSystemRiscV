#include "../../../include/kernelSpace/arch/riscV/htrap.h"

void handler_trap(struct trap_frame *tf) {
	struct scause_t scause = read_csr_scause();
	uint32_t stval = read_csr_stval();
	uint32_t sepc = read_csr_sepc();

	PANIC("\nException!Interrupt = %x, exception code = %x(scause)\nInformation specific to this exception = %x(stval)\nException address = %x(sepc)\n", (uint32_t)scause.interrupt, (uint32_t)scause.wlrl, stval, sepc);
}

