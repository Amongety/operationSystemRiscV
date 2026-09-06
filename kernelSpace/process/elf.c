#include "../../include/kernelSpace/process/elf.h"

int loadElf64(char* buff, uint32_t size, struct Elf64ProgramHeader** res, uint64_t* pc) {
    struct Elf64Header* headerELF64 = (struct Elf64Header*)buff;

    if(headerELF64->magic != 0x464C457F) {
        console_printf("Erorr MAGIC ELF64\r\n");
        return -1;
    }

    if(headerELF64->class != HEADER_CLASS_ELF64) {
        console_printf("Error CLASS ELF64\r\n");
        return -1;
    }

    if(headerELF64->machine != HEADER_MACHINE_RISCV) {
        console_printf("Error MACHINE ELF64\r\n");
        return -1;
    }

    if(headerELF64->type != HEADER_TYPE_EXEC) {
        console_printf("Error TYPE ELF64\r\n");
    }

    *res = (struct Elf64ProgramHeader*)(buff + headerELF64->phoff);
    *pc = headerELF64->entry;

    return headerELF64->phnum;
}