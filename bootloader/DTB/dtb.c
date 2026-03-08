#include "../../include/bootloader/DTB/dtb.h"

void pchis(uint64_t);
void phex(uint64_t);

bool init_dtb(uint8_t *dtb) {
	struct fdt_header* dtbHeader = (struct fdt_header*)dtb;

	if(bswap32(dtbHeader->magic) != 0xd00dfeed) {
		sbi_debug_console_write(23, (unsigned long*)"Error magic header DTB\n");
		return false;
	}

	uint32_t version = bswap32(dtbHeader->version);

	uint32_t offStruct = bswap32(dtbHeader->off_dt_struct);
	uint32_t sizeStruct = bswap32(dtbHeader->size_dt_struct);
	uint8_t *structureBlock = dtb + offStruct;

	uint32_t offString = bswap32(dtbHeader->off_dt_strings);
	uint32_t sizeString = bswap32(dtbHeader->size_dt_strings);
	uint8_t *structureString = dtb + offString;

	uint32_t offMMIO = bswap32(dtbHeader->off_mem_rsvmap);
	uint8_t *structureMMIO = dtb + offMMIO;

	memset((void*)&dtbPlt, 0, sizeof(struct dtbPlatform));

	switch(version) {
		case 17: {
			uint32_t token = bswap32(*((uint32_t*)structureBlock));
			bool uart = false;

			while(token != FDT_END) {
				token = bswap32(*((uint32_t*)structureBlock));
				structureBlock += 4;

				if(token == FDT_BEGIN_NODE) {
					uint8_t* name = structureBlock;

					/*while(*name != '\0') {
						sbi_debug_console_write_byte(*name);
						++name;
					}*/

					if(!strncmp(name, "uart", 4) || !strncmp(name, "serial", 6)) uart = true;

					structureBlock += strlen(name);
					structureBlock = (uint8_t*)(((uint64_t)structureBlock + 3) & ~3);

					//sbi_debug_console_write_byte('\n');
				}

				else if(token == FDT_PROP) {
					struct fdt_prop* prop = (struct fdt_prop*)structureBlock;
					uint32_t nameoff = bswap32(prop->nameoff);
					uint32_t len = bswap32(prop->len);

					if(len != 0) {
						bool add = false;

						structureBlock += sizeof(struct fdt_prop);
						uint8_t* name = structureString + nameoff;

						switch(*name) {
							case 'r': {
								if(!strncmp(name, "reg", 3) && uart) {
									uint64_t addr = bswap64(*((uint64_t*)structureBlock));
									uint64_t size = bswap64(*((uint64_t*)(structureBlock + sizeof(uint64_t))));

									for(int i = 0; i < UART_MAX; ++i) {
										if(dtbPlt.uart[i].addr == 0 && dtbPlt.uart[i].size == 0) {
											dtbPlt.uart[i].addr = addr;
											dtbPlt.uart[i].size = size;
											add = true;
											break;
										}
									}

									if(!add) sbi_debug_console_write(29, (unsigned long*)"Error Uart. Max supported OS\n");
								}
								break;
							}
						}

						/*for(uint8_t* start = structureString + nameoff; *start != '\0'; ++start) {
							sbi_debug_console_write_byte(*start);
						}
						
						structureBlock += sizeof(struct fdt_prop);
						sbi_debug_console_write(2, (unsigned long*)": ");

						if(len == 4) {
    							uint32_t v = bswap32(*(uint32_t*)structureBlock);
   			 				phex(v);
						}
						else if(len == 8) {
    							uint64_t v = bswap64(*(uint64_t*)structureBlock);
    							phex(v);
						}
						else {
    							for(uint32_t i = 0; i < len; i++) {
        						phex(structureBlock[i]);
        						sbi_debug_console_write_byte(' ');
    							}
						}*/

					}

					structureBlock += len;
					structureBlock = (uint8_t*)(((uint64_t)structureBlock + 3) & ~3);

					//sbi_debug_console_write_byte('\n');
				}

				else if(token == FDT_END_NODE) {
					uart = false;
				}
			}
			break;
		}

		default:
			sbi_debug_console_write(19, (unsigned long*)"Error version DTB: ");
			sbi_debug_console_write_byte(version | 0x30);
			sbi_debug_console_write_byte((version % 10) | 0x30);
			sbi_debug_console_write_byte('\n');
			return false;
	};

	return true;
}

void phex(uint64_t s) {
	for (int i = 1; i >= 0; i--) {
		uint64_t nibble = (s >> (i * 4)) & 0xf;
		sbi_debug_console_write_byte("0123456789abcdef"[nibble]);
        }		
}

void pchis(uint64_t s) {
	uint64_t r = s;
	uint64_t div = 1;
	
	while(r / div > 9) div *= 10;

        while(div) {
		sbi_debug_console_write_byte((r / div % 10) | 0x30);

                div /= 10;
	}
 
}

