#include "../../include/kernelSpace/arch/riscV/systemCalls/kexit.h"

void do_exit() {
	delete_process(&(bufProc[0]));

	struct Process* curProc = schedule();

	switchProc(curProc->frame, curProc->frame->csr_reg.epc, curProc->frame->csr_reg.atp); // (0x8000000000000000 | (newProc->pid << 44) | (((uint64_t)newProc->root) >> 12));
}