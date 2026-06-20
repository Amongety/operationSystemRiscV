#include "../../include/kernelSpace/filesystem/fs.h"

void fsInitMinix3() {
	parsingMBRPart(131, &LBA);
	uint8_t buff[1024], zmap[1024], imap[1024];

	readSDMMC(globalDTB.sd.addr, NONDMA, LBA + 2, (void*)buff, 512);
	memcpy(&superBlock, buff, sizeof(struct SuperBlockMinix3));
	offsetBlock = superBlock.block_size / 512;

	if(superBlock.magic == MAGIC_MINIX_3) {
		console_printf("\r\nMINIX 3\r\n");

		/*readSDMMC(globalDTB.sd.addr, NONDMA, LBA + (2 + superBlock.imap_blocks) * offsetBlock, (void*)zmap, 512);
		console_printf("\r\nZONE MAP\r\n");
		for(int i = 0; i < 512; ++i) {
			console_printf("byte: %d, zmap: %d\r\n", i, zmap[i]);
		}

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + 2 * offsetBlock, (void*)imap, 512);
		console_printf("\r\nINODE MAP\r\n");
		for(int i = 0; i < 512; ++i) {
			console_printf("byte: %d, imap: %d\r\n", i, imap[i]);
		}*/

		console_printf("ninodes: %d\r\nimap_blocks: %d\r\nzmap_blocks: %d\r\nfirst_data_zone: %d\r\nlog_zone_size: %d\r\nmax_size: %d\r\nzones: %d\r\nmagic: %d\r\nblock_size: %d\r\ndisk_version: %d\r\n", superBlock.ninodes, superBlock.imap_blocks, superBlock.zmap_blocks, superBlock.first_data_zone, superBlock.log_zone_size, superBlock.max_size, superBlock.zones, superBlock.magic, superBlock.block_size, superBlock.disk_version);
	}
}

struct InodeMinix3 getInodeMinix3(long inode_num) {
	struct InodeMinix3 inodeByte[superBlock.block_size / sizeof(struct InodeMinix3)];

	readSDMMC(globalDTB.sd.addr, NONDMA, LBA + ((2 + superBlock.imap_blocks + superBlock.zmap_blocks + ((inode_num - 1) / (superBlock.block_size / sizeof(struct InodeMinix3)))) * offsetBlock), (void*)inodeByte, superBlock.block_size);

	long inodeBit = (inode_num - 1) % (superBlock.block_size / sizeof(struct InodeMinix3));

	struct InodeMinix3 inode = inodeByte[inodeBit];

	/*console_printf("\r\nKAKOYTO FILE\r\n");
	console_printf("mode: %d\r\nnlinks: %d\r\nuid: %d\r\ngid: %d\r\nsize: %d\r\nattime: %d\r\nmtime: %d\r\nctime: %d\r\n", inode.mode, inode.nlinks, inode.uid, inode.gid, inode.size, inode.attime, inode.mtime, inode.ctime);
	for(int i = 0; i < 10; ++i) {
		console_printf("zone %d: %d\r\n", i, inode.zone[i]);
	}*/

	return inodeByte[inodeBit];
}

uint32_t processingZone(struct InodeMinix3 inode, long logBlck, uint32_t* buff) {
	uint32_t listSize = superBlock.block_size / sizeof(uint32_t);

	if(logBlck < 7) {
		return inode.zone[logBlck];
	}
	else if(logBlck < 7 + listSize) {
		if(inode.zone[7] == 0) return 0;

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + inode.zone[7] * offsetBlock, (void*)buff, superBlock.block_size);

		return buff[logBlck - 7];
	}
	else if(logBlck < 7 + listSize * listSize) {
		if(inode.zone[8] == 0) return 0;

		uint32_t curBlock = logBlck - 7 - listSize;
		uint32_t lvl2 = curBlock / listSize;
		uint32_t dataBlock = curBlock % listSize;

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + inode.zone[8] * offsetBlock, (void*)buff, superBlock.block_size);
		uint32_t zone1 = buff[lvl2];
		if(zone1 == 0) return 0;

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + zone1 * offsetBlock, (void*)buff, superBlock.block_size);

		return buff[dataBlock];
	}
	else if(logBlck < 7 + listSize * listSize * listSize) {
		if(inode.zone[9] == 0) return 0;

		uint32_t offset = logBlck - 7 - listSize - listSize * listSize;
		uint32_t lvl1 = offset / (listSize * listSize);
		uint32_t lvl2 = (offset / listSize) % listSize;
		uint32_t dataBlock = offset % listSize;

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + inode.zone[9] * offsetBlock, (void*)buff, superBlock.block_size);
		uint32_t zone1 = buff[lvl1];
		if(zone1 == 0) return 0;

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + zone1 * offsetBlock, (void*)buff, superBlock.block_size);
		uint32_t zone2 = buff[lvl2];
		if(zone2 == 0) return 0;

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + zone2 * offsetBlock, (void*)buff, superBlock.block_size);

		return buff[dataBlock];
	}

	return 0;
}

long searchPath(const char* path, long inode_num) {
	int sizeDir = superBlock.block_size / sizeof(struct DirEntryMinix3);
	struct InodeMinix3 inode = getInodeMinix3(inode_num);
	struct DirEntryMinix3 dir[sizeDir];
	uint32_t buffer[superBlock.block_size];

	switch(inode.mode & S_IFMT) {
		case S_IFREG:
		case S_IFDIR:
			for(int blk = 0, len = 0; len < inode.size && blk < superBlock.zones; ++blk) {
				uint64_t idx = processingZone(inode, blk, buffer);
				if(idx == 0) continue;

				readSDMMC(globalDTB.sd.addr, NONDMA, LBA + idx * offsetBlock, (void*)dir, superBlock.block_size);

				for(int i = 0; i < sizeDir; ++i) {
					if(dir[i].inode != 0) {
						len += sizeof(struct DirEntryMinix3);
						console_printf("Name file %s\r\n", dir[i].name);
						if(!strcmp(dir[i].name, path)) return dir[i].inode;
					}
				}
			}
			break;
	};

	return -1;
}

long depthMinix3(const char* path) {
	char *buf, name[LEN_NAME_DIR_ENTRY];
	long index = 0, inode_num = 1;

	while((buf = strchr(path + index, '/')) != NULL) {
		int i = buf - (path + index), p = 0;

		for(char* t = path + index; t < path + index + i; ++t, ++p) name[p] = *t;
		name[p] = '\0';

		index += i + 1;

		if((inode_num = searchPath(name, inode_num)) == -1) return -1;
	}

	return searchPath(path + index, inode_num);
}

long openMinix3(const char* path) {
	return depthMinix3(path);
}
