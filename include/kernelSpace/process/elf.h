#ifndef __ELF_H__
#define __ELF_H__

#include "../filesystem/fs.h"
#include "../arch/riscV/memory/mmu.h"

#define HEADER_CLASS_ELF64 0x2

#define HEADER_MACHINE_RISCV 0xF3

#define HEADER_TYPE_EXEC 0x2

#define PH_EXECUTE 1
#define PH_WRITE 2
#define PH_READ 4

#define PH_TYPE_NULL 0
#define PH_TYPE_LOAD 1
#define PH_TYPE_DYNAMIC 2
#define PH_TYPE_INTERP 3
#define PH_TYPE_NOTE 4


struct Elf64Header {
    uint32_t magic;
    uint8_t class;                      /* 32(1)- или 64(2)-битный формат */
    uint8_t data;                       /* little(1, обратный) или big(2, прямой) порядок байт */
    uint8_t ident_abi_version;
    uint8_t os_abi;                     /* id abi */
    uint8_t abi_version;                /* версия abi */
    uint8_t pad[7];
    uint16_t type;			            /* Тип объектного файла */
    uint16_t machine;		            /* Архитектура */
    uint32_t version;		            /* Версия объектного файла */
    uint64_t entry;		                /* Виртуальный адрес точки входа */
    uint64_t phoff;		                /* Смещение файла таблицы заголовке program */
    uint64_t shoff;		                /* Смещение файла таблицы заголовке section */
    uint32_t flags;		                /* Флаг */
    uint16_t ehsize;		            /* Размер заголовка ELF в байтах */
    uint16_t phentsize;		            /* Размер записи таблицы заголовке program */
    uint16_t phnum;		                /* Количество записей в таблице заголовке program */
    uint16_t shentsize;		            /* Размер записи таблицы заголовка section */
    uint16_t shnum;		                /* Количество записей в таблице заголовке section */
    uint16_t shstrndx;		            /* Индекс таблицы строк заголовка section */
};

struct Elf64ProgramHeader {
    uint32_t type;                      /* Тип сегмента */
    uint32_t flags;                     /* Флаг сегмента */
    uint64_t offset;
    uint64_t vaddr;
    uint64_t paddr;
    uint64_t filesz;                    /* Размер сегмента изображения файла в байтах */
    uint64_t memsz;                     /* Размер сегмента в памяти в байтах */
    uint64_t allign;                    /* 0 - нет выравнивания, 1 - есть */
};

int loadElf64(char* buff, uint32_t size, struct Elf64ProgramHeader** res, uint64_t* pc);

#endif