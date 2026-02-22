#include "../../../include/kernelSpace/arch/riscV/htrap.h"

uint64_t handler_trap(struct trap_frame *tf) {
	uint64_t next_epc = tf->csr_reg.epc;

	if(tf->csr_reg.cause.interrupt == 1) {
		switch(tf->csr_reg.cause.exceptCode) {
			case 1:
				console_printf("\nSupervisor software interrupt 0x%x: 0x%x\r\n\r\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 5: 
			{
				uint64_t sie = read_csr_sie();
				write_csr_sie(sie & (~0x20));

				if(prevProcess) *(prevProcess->frame) = *tf; 
				
				struct Process curProc = schedule();
				
				sbi_set_timer(10000000);

				switchProc(curProc.frame, curProc.frame->csr_reg.epc, curProc.frame->csr_reg.atp); // (0x8000000000000000 | (newProc->pid << 44) | (((uint64_t)newProc->root) >> 12));

				break;
			}

			case 9:
				console_printf("\r\nSupervisor external interrupt 0x%x: 0x%x\r\n\r\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 13:
				console_printf("\r\nCounter-overflow interrupt 0x%x: 0x%x\r\n\r\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			default:
				PANIC("\r\nException async! Interrupt = 0x%x, exception code = 0x%x(scause)\r\nInformation specific to this exception = 0x%x(stval)\r\nException address = 0x%x(sepc)\r\n\r\n", (uint64_t)tf->csr_reg.cause.interrupt, (uint64_t)tf->csr_reg.cause.exceptCode, tf->csr_reg.tval, tf->csr_reg.epc);
		};
	}

	else {
		switch(tf->csr_reg.cause.exceptCode) {
			case 0:
				PANIC("\r\nInstruction address misaligned 0x%x: 0x%x\r\n\r\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 1:
				PANIC("\r\nInstruction access fault 0x%x: 0x%x\r\n\r\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;
			
			case 2:
				PANIC("\r\nIllegal instruction 0x%x: 0x%x\r\n\r\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;
			
			case 3:
				PANIC("\r\nBreakpoint 0x%x: 0x%x\r\n\r\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 4:
				PANIC("\r\nLoad address misaligned 0x%x: 0x%x\r\n\r\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 5:
				PANIC("\r\nLoad access fault 0x%x: 0x%x\r\n\r\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 6:
				PANIC("\r\nStore/AMO address misaligned 0x%x: 0x%x\r\n\r\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 7:
				PANIC("\r\nStore/AMO access fault 0x%x: 0x%x\r\n\r\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 8:
				console_printf("\r\nCall. Environment call (ecall) from U-mode 0x%x\r\n\r\n", tf->csr_reg.epc);
				next_epc = do_syscall(tf);
				break;

			case 9:
				console_printf("\r\nEnvironment call (ecall) from S-mode! 0x%x\r\n\r\n", tf->csr_reg.epc);
				next_epc += 4;
				break;

			case 12:
				PANIC("\r\nInstruction page fault 0x%x: 0x%x\r\n\r\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 13:
				PANIC("\r\nLoad page fault 0x%x: 0x%x\r\n\r\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 15:
				PANIC("\r\nStore/AMO page fault 0x%x: 0x%x\r\n\r\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 16:
				PANIC("\r\nDouble trap 0x%x: 0x%x\r\n\r\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 18:
				PANIC("\r\nSoftware check 0x%x: 0x%x\r\n\r\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 19:
				PANIC("\r\nHardware error 0x%x: 0x%x\r\n\r\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			default:
				PANIC("\r\nException sync! Interrupt = 0x%x, exception code = 0x%x(scause)\r\nInformation specific to this exception = 0x%x(stval)\r\nException address = 0x%x(sepc)\r\n\r\n", (uint64_t)tf->csr_reg.cause.interrupt, (uint64_t)tf->csr_reg.cause.exceptCode, tf->csr_reg.tval, tf->csr_reg.epc);
		};
	}

	return next_epc;
}

