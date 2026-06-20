#ifndef __PARSING_DISK_H__
#define __PARSING_DISK_H__

#include "../drivers/sdmmc/sdmmc.h"

extern struct dtbPlatform globalDTB;

struct MBRPartition {
	uint32_t status:8;
	uint32_t CHSfirst:24;
	uint32_t partitionType:8;
	uint32_t CHSlast:24;
	uint32_t LBA;
	uint32_t numberSectors;
};

void parsingMBRPart(uint8_t type, uint32_t* LBA);

#endif
