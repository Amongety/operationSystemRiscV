#include "../../../../include/kernelSpace/arch/riscV/systemCalls/syscall.h"

uint64_t do_syscall(struct trap_frame* tf) {
	switch(tf->a7) {
		case SYSCALL_EXIT:
			do_exit();

			break;
		default:
			sbi_console_printf("Indefinite system calls %d\n", tf->a7);
			return tf->csr_reg.epc + 4;
	}
}

void do_exit() {
	delete_process(&(bufProc[0]));

	struct Process curProc = schedule();

	switchProc(curProc.frame, curProc.frame->csr_reg.epc, curProc.frame->csr_reg.atp); // (0x8000000000000000 | (newProc->pid << 44) | (((uint64_t)newProc->root) >> 12));
}
