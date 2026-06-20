#include "../../include/kernelSpace/filesystem/parsingDisk.h"

void parsingMBRPart(uint8_t type, uint32_t* LBA) {
	uint8_t buff[512];
	while(!readSDMMC(globalDTB.sd.addr, NONDMA, 0, (void*)buff, 512)) {}
	
	bool find = false;
	struct MBRPartition *mbrPart = (struct MBRPartition*)(buff + 446); 
	for(int i = 0; i < 4; ++i) {
		if(mbrPart[i].partitionType == type) {
			find = true;
			*LBA = mbrPart[i].LBA;
			break;
		}
		//console_printf("\r\n--Partition %d--\r\nStatus drive: %d\r\nCHS first: %d\r\nPartition type: %d\r\nCHS last: %d\r\nLBA: %d\r\nNumber sectors: %d\r\n", i, mbrPart[i].status, mbrPart[i].CHSfirst, mbrPart[i].partitionType, mbrPart[i].CHSlast, mbrPart[i].LBA, mbrPart[i].numberSectors);
	}

	if(!find) PANIC("Not found partition");
}

