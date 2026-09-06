#include "../../../../include/kernelSpace/arch/riscV/systemCalls/syscall.h"

uint64_t do_syscall(struct TrapFrame* tf) {
	switch(tf->a7) {
		case SYSCALL_EXIT:
			do_exit();

			break;
		case 448: 																											// 't'+'e'+'s'+'t'
		{
			uint64_t old_sstatus;

			__asm__ volatile(
				"csrr %0, sstatus"
				: "=r"(old_sstatus)
			);

			__asm__ volatile(
				"li t0, (1 << 18)\n"
				"csrs sstatus, t0\n"
				:
				:
				: "t0", "memory"
			);

			console_printf((const unsigned char*)tf->a0, tf->a1, tf->a2, tf->a3, tf->a4, tf->a5, tf->a6);

			__asm__ volatile(
				"csrw sstatus, %0"
				:
				: "r"(old_sstatus)
				: "memory"
			);
			
			return tf->csr_reg.epc + 4;

			break;
		}
		default:
			console_printf("Indefinite system calls %d\r\n", tf->a7);
			return tf->csr_reg.epc + 4;
	}
}

void do_exit() {
	delete_process(&(bufProc[0]));

	struct Process* curProc = schedule();

	switchProc(curProc->frame, curProc->frame->csr_reg.epc, curProc->frame->csr_reg.atp); // (0x8000000000000000 | (newProc->pid << 44) | (((uint64_t)newProc->root) >> 12));
}
