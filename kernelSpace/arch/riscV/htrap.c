#include "../../../include/kernelSpace/arch/riscV/htrap.h"

uint32_t handler_trap(struct trap_frame *tf) {
	uint32_t next_epc = tf->csr_reg.epc;

	if(tf->csr_reg.cause.interrupt == 1) {
		switch(tf->csr_reg.cause.exceptCode) {
			case 1:
				sbi_console_printf("\nSupervisor software interrupt 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 5:
				sbi_console_printf("\nSupervisor timer interrupt 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 9:
				sbi_console_printf("\nSupervisor external interrupt 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 13:
				sbi_console_printf("\nCounter-overflow interrupt 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			default:
				PANIC("\nException async! Interrupt = 0x%x, exception code = 0x%x(scause)\nInformation specific to this exception = 0x%x(stval)\nException address = 0x%x(sepc)\n\n", (uint32_t)tf->csr_reg.cause.interrupt, (uint32_t)tf->csr_reg.cause.exceptCode, tf->csr_reg.tval, tf->csr_reg.epc);
		};
	}

	else {
		switch(tf->csr_reg.cause.exceptCode) {
			case 0:
				PANIC("\nInstruction address misaligned 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 1:
				PANIC("\nInstruction access fault 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;
			
			case 2:
				PANIC("\nIllegal instruction 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;
			
			case 3:
				PANIC("\nBreakpoint 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 4:
				PANIC("\nLoad address misaligned 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 5:
				PANIC("\nLoad access fault 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 6:
				PANIC("\nStore/AMO address misaligned 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 7:
				PANIC("\nStore/AMO access fault 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 8:
				sbi_console_printf("\nEnvironment call (ecall) from U-mode! 0x%x\n\n", tf->csr_reg.epc);
				next_epc += 4;
				break;

			case 9:
				sbi_console_printf("\nEnvironment call (ecall) from S-mode! 0x%x\n\n", tf->csr_reg.epc);
				next_epc += 4;
				break;

			case 12:
				sbi_console_printf("\nInstruction page fault 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				next_epc += 4;
				break;

			case 13:
				sbi_console_printf("\nLoad page fault 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				next_epc += 4;
				break;

			case 15:
				sbi_console_printf("\nStore/AMO page fault 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				next_epc += 4;
				break;

			case 16:
				PANIC("\nDouble trap 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 18:
				PANIC("\nSoftware check 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			case 19:
				PANIC("\nHardware error 0x%x: 0x%x\n\n", tf->csr_reg.epc, tf->csr_reg.tval);
				break;

			default:
				PANIC("\nException sync! Interrupt = 0x%x, exception code = 0x%x(scause)\nInformation specific to this exception = 0x%x(stval)\nException address = 0x%x(sepc)\n\n", (uint32_t)tf->csr_reg.cause.interrupt, (uint32_t)tf->csr_reg.cause.exceptCode, tf->csr_reg.tval, tf->csr_reg.epc);
		};
	}

	return next_epc;
}

