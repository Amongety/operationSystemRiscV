#include "../../include/kernelSpace/filesystem/fs.h"

uint32_t getInodeSize(long inode_num) {
	struct InodeMinix3 inode = getInodeMinix3(inode_num);
	return inode.size;
}

void fsInitMinix3() {
	parsingMBRPart(131, &LBA);
	uint8_t buff[1024], zmap[1024], imap[1024];

	readSDMMC(globalDTB.sd.addr, NONDMA, LBA + 2, (void*)buff, 1024);
	memcpy(&superBlock, buff, sizeof(struct SuperBlockMinix3));
	offsetBlock = superBlock.block_size / BLOCK_BYTE;

	if(superBlock.magic == MAGIC_MINIX_3) {
		console_printf("\r\nMINIX 3\r\n");
		console_printf("LBA: %d\r\nninodes: %d\r\nimap_blocks: %d\r\nzmap_blocks: %d\r\nfirst_data_zone: %d\r\nlog_zone_size: %d\r\nmax_size: %d\r\nzones: %d\r\nmagic: %d\r\nblock_size: %d\r\ndisk_version: %d\r\n", LBA, superBlock.ninodes, superBlock.imap_blocks, superBlock.zmap_blocks, superBlock.first_data_zone, superBlock.log_zone_size, superBlock.max_size, superBlock.zones, superBlock.magic, superBlock.block_size, superBlock.disk_version);
	}
}

struct InodeMinix3 getInodeMinix3(long inode_num) {
	struct InodeMinix3 inodeByte[superBlock.block_size / sizeof(struct InodeMinix3)];

	readSDMMC(globalDTB.sd.addr, NONDMA, LBA + ((2 + superBlock.imap_blocks + superBlock.zmap_blocks + ((inode_num - 1) / (superBlock.block_size / sizeof(struct InodeMinix3)))) * offsetBlock), (void*)inodeByte, superBlock.block_size);

	long inodeBit = (inode_num - 1) % (superBlock.block_size / sizeof(struct InodeMinix3));

	struct InodeMinix3 inode = inodeByte[inodeBit];

	return inodeByte[inodeBit];
}

void putInodeMinix3(long inode_num, struct InodeMinix3 src) {
	struct InodeMinix3 inodeByte[superBlock.block_size / sizeof(struct InodeMinix3)];
	uint64_t index = LBA + ((2 + superBlock.imap_blocks + superBlock.zmap_blocks + ((inode_num - 1) / (superBlock.block_size / sizeof(struct InodeMinix3)))) * offsetBlock);

	readSDMMC(globalDTB.sd.addr, NONDMA, index, (void*)inodeByte, superBlock.block_size);

	long inodeBit = (inode_num - 1) % (superBlock.block_size / sizeof(struct InodeMinix3));

	memcpy((void*)&inodeByte[inodeBit], (void*)&src, sizeof(struct InodeMinix3));

	writeSDMMC(globalDTB.sd.addr, NONDMA, index, (void*)inodeByte, superBlock.block_size);
}

uint32_t processingZoneMinix3(struct InodeMinix3 inode, long logBlck, uint32_t* buff) {
	uint64_t listSize = superBlock.block_size / sizeof(uint32_t);

	if(logBlck < 7) {
		return inode.zone[logBlck];
	}
	else if(logBlck < 7 + listSize) {
		if(inode.zone[7] == 0) return 0;

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)inode.zone[7] * offsetBlock, (void*)buff, superBlock.block_size);

		return buff[logBlck - 7];
	}
	else if(logBlck < 7 + listSize + listSize * listSize) {
		if(inode.zone[8] == 0) return 0;

		uint64_t curBlock = logBlck - 7 - listSize;
		uint64_t lvl2 = curBlock / listSize;
		uint64_t dataBlock = curBlock % listSize;

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)inode.zone[8] * offsetBlock, (void*)buff, superBlock.block_size);
		uint64_t zone1 = buff[lvl2];
		if(zone1 == 0) return 0;

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + zone1 * offsetBlock, (void*)buff, superBlock.block_size);

		return buff[dataBlock];
	}
	else if(logBlck < 7 + listSize + listSize * listSize + listSize * listSize * listSize){
		if(inode.zone[9] == 0) return 0;

		uint64_t offset = logBlck - 7 - listSize - listSize * listSize;
		uint64_t lvl1 = offset / (listSize * listSize);
		uint64_t lvl2 = (offset / listSize) % listSize;
		uint64_t dataBlock = offset % listSize;

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)inode.zone[9] * offsetBlock, (void*)buff, superBlock.block_size);
		uint64_t zone1 = buff[lvl1];
		if(zone1 == 0) return 0;

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + zone1 * offsetBlock, (void*)buff, superBlock.block_size);
		uint64_t zone2 = buff[lvl2];
		if(zone2 == 0) return 0;

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + zone2 * offsetBlock, (void*)buff, superBlock.block_size);

		return buff[dataBlock];
	}

	return 0;
}

uint32_t ensureZoneMinix3(struct InodeMinix3* inode, long logBlck, uint32_t* buff) {
	uint8_t zeroBuf[superBlock.block_size];
	memset(zeroBuf, 0, superBlock.block_size);
	uint64_t listSize = superBlock.block_size / sizeof(uint32_t);

	if(logBlck < 7) {
		if(inode->zone[logBlck] == 0) {
			inode->zone[logBlck] = allocZoneMinix3();
			writeSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)inode->zone[logBlck] * offsetBlock, (void*)zeroBuf, superBlock.block_size);
		}

		return inode->zone[logBlck];
	}
	else if(logBlck < 7 + listSize) {
		if(inode->zone[7] == 0) {
			inode->zone[7] = allocZoneMinix3();
			writeSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)inode->zone[7] * offsetBlock, (void*)zeroBuf, superBlock.block_size);
		}

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)inode->zone[7] * offsetBlock, (void*)buff, superBlock.block_size);

		if(buff[logBlck - 7] == 0) {
			buff[logBlck - 7] = allocZoneMinix3();
			writeSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)inode->zone[7] * offsetBlock, (void*)buff, superBlock.block_size);
		}

		return buff[logBlck - 7];
	}
	else if(logBlck < 7 + listSize + listSize * listSize) {
		if(inode->zone[8] == 0) {
			inode->zone[8] = allocZoneMinix3();
			writeSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)inode->zone[8] * offsetBlock, (void*)zeroBuf, superBlock.block_size);
		}

		uint64_t curBlock = logBlck - 7 - listSize;
		uint64_t lvl2 = curBlock / listSize;
		uint64_t dataBlock = curBlock % listSize;

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)inode->zone[8] * offsetBlock, (void*)buff, superBlock.block_size);
	
		uint64_t zone1 = buff[lvl2];
		if(zone1 == 0) {
			zone1 = allocZoneMinix3();
			buff[lvl2] = zone1;
			writeSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)inode->zone[8] * offsetBlock, (void*)buff, superBlock.block_size);
		}

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + zone1 * offsetBlock, (void*)buff, superBlock.block_size);

		if(buff[dataBlock] == 0) {
			buff[dataBlock] = allocZoneMinix3();
			writeSDMMC(globalDTB.sd.addr, NONDMA, LBA + zone1 * offsetBlock, (void*)buff, superBlock.block_size);
		}

		return buff[dataBlock];
	}
	else if(logBlck < 7 + listSize + listSize * listSize + listSize * listSize * listSize){
		if(inode->zone[9] == 0) {
			inode->zone[9] = allocZoneMinix3();
			writeSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)inode->zone[9] * offsetBlock, (void*)zeroBuf, superBlock.block_size);
		}

		uint64_t offset = logBlck - 7 - listSize - listSize * listSize;
		uint64_t lvl1 = offset / (listSize * listSize);
		uint64_t lvl2 = (offset / listSize) % listSize;
		uint64_t dataBlock = offset % listSize;

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)inode->zone[9] * offsetBlock, (void*)buff, superBlock.block_size);

		uint32_t zone1 = buff[lvl1];
		if(zone1 == 0) {
			zone1 = allocZoneMinix3();
			buff[lvl1] = zone1;
			writeSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)inode->zone[9] * offsetBlock, (void*)buff, superBlock.block_size);
		}

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + zone1 * offsetBlock, (void*)buff, superBlock.block_size);

		uint64_t zone2 = buff[lvl2];
		if(zone2 == 0) {
			zone2 = allocZoneMinix3();
			buff[lvl2] = zone2;
			writeSDMMC(globalDTB.sd.addr, NONDMA, LBA + zone1 * offsetBlock, (void*)buff, superBlock.block_size);
		}

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + zone2 * offsetBlock, (void*)buff, superBlock.block_size);

		if(buff[dataBlock] == 0) {
			buff[dataBlock] = allocZoneMinix3();
			writeSDMMC(globalDTB.sd.addr, NONDMA, LBA + zone2 * offsetBlock, (void*)buff, superBlock.block_size);
		}

		return buff[dataBlock];
	}

	return 0;
}

void deleteZonesMinix3(struct InodeMinix3 inode, uint32_t* buff) {
    uint64_t countBlk = (inode.size + superBlock.block_size - 1) / superBlock.block_size;
    uint64_t listSize = superBlock.block_size / sizeof(uint32_t);

    for(int i = 0; countBlk > 0 && i < 7; ++i) {
        if(inode.zone[i]) {
            deallocZmapMinix3(inode.zone[i]);
            --countBlk;
        }
    }

    if(countBlk > 0 && inode.zone[7]) {
        readSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)inode.zone[7] * offsetBlock, (void*)buff, superBlock.block_size);

        for(int i = 0; countBlk > 0 && i < listSize; ++i) {
            if(buff[i]) {
                deallocZmapMinix3(buff[i]);
                --countBlk;
            }
        }
        deallocZmapMinix3(inode.zone[7]);
    }

    if(countBlk > 0 && inode.zone[8]) {
        readSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)inode.zone[8] * offsetBlock, (void*)buff, superBlock.block_size);

        for(int i = 0; i < listSize; ++i) {
			if(buff[i] == 0) continue;
			
			uint32_t lvl2[listSize];

            readSDMMC(globalDTB.sd.addr, NONDMA, LBA + buff[i] * offsetBlock, (void*)lvl2, superBlock.block_size);

            for(int j = 0; countBlk > 0 && j < listSize; ++j) {
                if(lvl2[j]) {
                    deallocZmapMinix3(lvl2[j]);
                    --countBlk;
                }
            }
            deallocZmapMinix3(buff[i]);
        }
        deallocZmapMinix3(inode.zone[8]);
    }

    if(countBlk > 0 && inode.zone[9]) {
        readSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)inode.zone[9] * offsetBlock, (void*)buff, superBlock.block_size);

        for(int i = 0; i < listSize; ++i) {
            if(buff[i] == 0) continue;

			uint32_t lvl3[listSize];

            readSDMMC(globalDTB.sd.addr, NONDMA, LBA + buff[i] * offsetBlock, (void*)lvl3, superBlock.block_size);

            for(int j = 0; j < listSize; ++j) {
                if(lvl3[j] == 0) continue;

				uint32_t lvl2[listSize];

                readSDMMC(globalDTB.sd.addr, NONDMA, LBA + lvl3[j] * offsetBlock, (void*)lvl2, superBlock.block_size);

                for(int k = 0; countBlk > 0 && k < listSize; ++k) {
                    if(lvl2[k]) {
                        deallocZmapMinix3(lvl2[k]);
                        --countBlk;
                    }
                }
                deallocZmapMinix3(lvl3[j]);
            }
            deallocZmapMinix3(buff[i]);
        }
        deallocZmapMinix3(inode.zone[9]);
    }
}

uint32_t allocZoneMinix3() {
	uint8_t zmap[superBlock.block_size];

	for(uint32_t i = 0; i < superBlock.zmap_blocks; ++i) {
		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + (2 + superBlock.imap_blocks + i) * offsetBlock, (void*)zmap, superBlock.block_size);
		
		for(int byte = 0; byte < superBlock.block_size; ++byte) {
			if(zmap[byte] == 0xFF) continue;

			for(int bit = 0; bit < 8; ++bit) {
				uint32_t z = superBlock.first_data_zone - 1 + i * superBlock.block_size * 8 + byte * 8 + bit;

				if(z >= superBlock.zones) return 0;

				if(!(zmap[byte] & (1 << bit))) {
					zmap[byte] |= (1 << bit);
					writeSDMMC(globalDTB.sd.addr, NONDMA, LBA + (2 + superBlock.imap_blocks + i) * offsetBlock, (void*)zmap, superBlock.block_size);
					
					return z;
				}
			}
		}
	}

	return 0;
}

int deallocZmapMinix3(uint32_t zone) {
	if(zone < superBlock.first_data_zone) return -1;

	uint32_t z = zone - superBlock.first_data_zone + 1;
	uint32_t i = z / (superBlock.block_size * 8);
	uint32_t byte = (z % (superBlock.block_size * 8)) / 8;
	uint32_t bit = z % 8;

	if(i >= superBlock.zmap_blocks) return -1;

	uint8_t zmap[superBlock.block_size];
	uint64_t index = LBA + (2 + superBlock.imap_blocks + i) * offsetBlock;

	readSDMMC(globalDTB.sd.addr, NONDMA, index, zmap, superBlock.block_size);

	zmap[byte] &= ~(1 << bit);

	writeSDMMC(globalDTB.sd.addr, NONDMA, index, zmap, superBlock.block_size);

	return 0;
}

uint32_t allocImapMinix3() {
	uint8_t imap[superBlock.block_size];

	for(int i = 0; i < superBlock.imap_blocks; ++i) {
		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + (2 + i) * offsetBlock, (void*)imap, superBlock.block_size);

		for(int byte = 0; byte < superBlock.block_size; ++byte) {
			if(imap[byte] == 0xFF) continue;

			for(int bit = 0; bit < 8; ++bit) {
				uint32_t inode = i * (uint32_t)superBlock.block_size * 8 + byte * 8 + bit;
				
				if(inode >= superBlock.ninodes) return 0;

				if((!(imap[byte] & (1 << bit))) && (inode != 0)) {
					imap[byte] |= (1 << bit);
					writeSDMMC(globalDTB.sd.addr, NONDMA, LBA + (2 + i) * offsetBlock, (void*)imap, superBlock.block_size);

					return inode;
				}
			}
		}
	}

	return 0;
}

int deallocImapMinix3(long inode_num) {
	if(inode_num == 0) return -1;

	uint32_t i = (inode_num) / (superBlock.block_size * 8);
	uint32_t byte = ((inode_num) % (superBlock.block_size * 8)) / 8;
	uint32_t bit = (inode_num) % 8;

	if(i >= superBlock.imap_blocks) return -1;

	uint8_t imap[superBlock.block_size];
	uint64_t index = LBA + (2 + i) * offsetBlock;

	readSDMMC(globalDTB.sd.addr, NONDMA, index, (void*)imap, superBlock.block_size);
	imap[byte] &= ~(1 << bit);
	writeSDMMC(globalDTB.sd.addr, NONDMA, index, (void*)imap, superBlock.block_size);

	return 0;
}

int allocDirMinix3(uint32_t chInode, uint32_t pInode, const char* name) {
	int sizeDir = superBlock.block_size / sizeof(struct DirEntryMinix3);
	struct InodeMinix3 parentInode = getInodeMinix3(pInode);
	struct DirEntryMinix3 dir[sizeDir];
	uint8_t buffer[superBlock.block_size];
	uint32_t pBlock = (parentInode.size + superBlock.block_size - 1) / superBlock.block_size;
	uint64_t count = 0;
	int nameLen = strlen(name);
	
	if(nameLen >= LEN_NAME_DIR_ENTRY) return -1;

	for(int blk = 0; blk < pBlock; ++blk) {
		uint64_t idx = processingZoneMinix3(parentInode, blk, (uint32_t*)buffer);

		if(idx == 0) {
			idx = ensureZoneMinix3(&parentInode, blk, (uint32_t*)buffer);
			if(idx == 0) return -1;

			memset(dir, 0, superBlock.block_size);
		}
		else {
			readSDMMC(globalDTB.sd.addr, NONDMA, LBA + idx * offsetBlock, (void*)dir, superBlock.block_size);
		}

		for(int i = 0; i < sizeDir; ++i) {
			count += sizeof(struct DirEntryMinix3);

			if(dir[i].inode == 0) {
				for(int j = 0; j < nameLen; ++j) dir[i].name[j] = name[j];
                dir[i].name[nameLen] = '\0';
				dir[i].inode = chInode;
				
				writeSDMMC(globalDTB.sd.addr, NONDMA, LBA + idx * offsetBlock, (void*)dir, superBlock.block_size);

				if(parentInode.size < count) {
					parentInode.size = count;
					putInodeMinix3(pInode, parentInode);
				}

				return 0;
			}
		}
	}

	uint64_t idx = ensureZoneMinix3(&parentInode, pBlock, (uint32_t*)buffer);
	if(idx == 0) return -1;

	memset(dir, 0, superBlock.block_size);

	for(int j = 0; j < nameLen; ++j) dir[0].name[j] = name[j];
	dir[0].name[nameLen] = '\0';
	dir[0].inode = chInode;

	writeSDMMC(globalDTB.sd.addr, NONDMA, LBA + idx * offsetBlock, (void*)dir, superBlock.block_size);
	
	count += sizeof(struct DirEntryMinix3);

	if(parentInode.size < count) {
		parentInode.size = count;
		putInodeMinix3(pInode, parentInode);
	}

	return 0;
}

long searchPath(struct DirInfo* di) {
	int sizeDir = superBlock.block_size / sizeof(struct DirEntryMinix3);
	struct InodeMinix3 inode = getInodeMinix3(di->parentInode);
	struct DirEntryMinix3 dir[sizeDir];
	uint8_t buffer[superBlock.block_size];

	switch(inode.mode & S_IFMT) {
		case S_IFDIR:
			for(int blk = 0, len = 0; len < inode.size && blk < superBlock.zones; ++blk) {
				uint64_t idx = processingZoneMinix3(inode, blk, (uint32_t*)buffer);
				if(idx == 0) continue;

				readSDMMC(globalDTB.sd.addr, NONDMA, LBA + idx * offsetBlock, (void*)dir, superBlock.block_size);

				for(int i = 0; i < sizeDir; ++i) {
					len += sizeof(struct DirEntryMinix3);

					if(dir[i].inode != 0) {
						if(!strcmp(dir[i].name, di->name)) {
							di->indexDir = i;
							di->sector = idx;
							return dir[i].inode;
						}
					}
				}
			}
			break;
	};

	return -1;
}

long depthMinix3(const char* path, struct DirInfo* di) {
	char *buf;
	long index = 0, inode_num = 1;
	di->parentInode = 0;
	memcpy(di->name, "/", 2);

	while(path[index] == '/') ++index;
	if(path[index] == '\0') return inode_num;

	while((buf = strchr(path + index, '/')) != NULL) {
		int i = buf - (path + index), p = 0;

		di->parentInode = inode_num;

		for(const char* t = path + index; t < path + index + i; ++t, ++p) di->name[p] = *t;
		di->name[p] = '\0';

		index += i + 1;
		while(path[index] == '/') ++index;

		if(p == 0) continue;
		if((inode_num = searchPath(di)) == -1) return -1;
	}

	if(path[index] == '\0') return inode_num;

	di->parentInode = inode_num;

	int p = 0;
	for(const char* t = path + index; *t != '\0'; ++t, ++p) di->name[p] = *t;
	di->name[p] = '\0';

	return searchPath(di);
}

long openMinix3(const char* path, uint16_t mode) {
	struct DirInfo di;
	uint8_t buffer[superBlock.block_size];
	long chInode = depthMinix3(path, &di);

	if(chInode != -1) return chInode;

	struct InodeMinix3 parentInode = getInodeMinix3(di.parentInode);
    if((parentInode.mode & S_IFMT) != S_IFDIR) return -1;

	long inode = creatFileMinix3(di.name, di.parentInode, S_IFREG | mode);
	if(inode == -1) return -1;

	if(allocDirMinix3(inode, di.parentInode, di.name) == -1) {
		struct InodeMinix3 newInode = getInodeMinix3(inode);
        deleteZonesMinix3(newInode, (uint32_t*)buffer);

		deallocImapMinix3(inode);

		struct InodeMinix3 tmp = {0};
		putInodeMinix3(inode, tmp);

		return -1;
	}

	return inode;
}

long mkdirMinix3(const char* path, uint16_t mode) {
	struct DirInfo di;
	uint8_t buffer[superBlock.block_size];
	long chInode = depthMinix3(path, &di);

	if(chInode == -1) {
		struct InodeMinix3 parentInode = getInodeMinix3(di.parentInode);
    	if((parentInode.mode & S_IFMT) != S_IFDIR) return -1;

		long inode = creatFileMinix3(di.name, di.parentInode, S_IFDIR | mode);
		if(inode == -1) return -1;

		if(allocDirMinix3(inode, inode, ".") == -1) {
			deallocImapMinix3(inode);
		
			struct InodeMinix3 tmp = {0};
			putInodeMinix3(inode, tmp);

			return -1;
		}
		if(allocDirMinix3(di.parentInode, inode, "..") == -1) {
			struct InodeMinix3 newInode = getInodeMinix3(inode);
            deleteZonesMinix3(newInode, (uint32_t*)buffer);

			deallocImapMinix3(inode);
		
			struct InodeMinix3 tmp = {0};
			putInodeMinix3(inode, tmp);

			return -1;
		}
		
		if(allocDirMinix3(inode, di.parentInode, di.name) == -1) {
			struct InodeMinix3 newInode = getInodeMinix3(inode);
            deleteZonesMinix3(newInode, (uint32_t*)buffer);

			deallocImapMinix3(inode);
		
			struct InodeMinix3 tmp = {0};
			putInodeMinix3(inode, tmp);

			return -1;
		}

		++parentInode.nlinks;
		putInodeMinix3(di.parentInode, parentInode);

		return inode;
	}

	return -1;
}

int rmdirMinix3(const char* path) {
	struct DirInfo di;
	int sizeDir = superBlock.block_size / sizeof(struct DirEntryMinix3);
	struct DirEntryMinix3 dir[sizeDir];
	uint8_t buffer[superBlock.block_size];
	long chInode = depthMinix3(path, &di);

	if(chInode == -1) return -1;
	if(di.parentInode == 0) return -1;

	struct InodeMinix3 inode = getInodeMinix3(chInode);
	
	if((inode.mode & S_IFMT) != S_IFDIR) return -1;

	for(int blk = 0, len = 0; len < inode.size && blk < superBlock.zones; ++blk) {
		uint64_t idx = processingZoneMinix3(inode, blk, (uint32_t*)buffer);
		if(idx == 0) continue;

		readSDMMC(globalDTB.sd.addr, NONDMA, LBA + idx * offsetBlock, (void*)dir, superBlock.block_size);

		for(int i = 0; i < sizeDir; ++i) {
			len += sizeof(struct DirEntryMinix3);

			if(dir[i].inode != 0) {
				if((!strcmp(dir[i].name, ".")) && (!strcmp(dir[i].name, ".."))) {
					return -1;
				}
			}
		}
	}
	
	if(deleteDirMinix3(di) == -1) return -1;

	deleteZonesMinix3(inode, (uint32_t*)buffer);

	deallocImapMinix3(chInode);
	struct InodeMinix3 tmp = {0};
    putInodeMinix3(chInode, tmp);

	struct InodeMinix3 parentInode = getInodeMinix3(di.parentInode);
    --parentInode.nlinks;
    putInodeMinix3(di.parentInode, parentInode);

	return 0;
}

int deleteDirMinix3(struct DirInfo di) {
	int sizeDir = superBlock.block_size / sizeof(struct DirEntryMinix3);
	struct DirEntryMinix3 dir[sizeDir];

	readSDMMC(globalDTB.sd.addr, NONDMA, LBA + di.sector * offsetBlock, (void*)dir, superBlock.block_size);

	dir[di.indexDir].inode = 0;
	memset(dir[di.indexDir].name, 0, LEN_NAME_DIR_ENTRY);

	writeSDMMC(globalDTB.sd.addr, NONDMA, LBA + di.sector * offsetBlock, (void*)dir, superBlock.block_size);

	return 0;
}

int unlinkMinix3(const char* path) {
	struct DirInfo di;
	uint8_t buffer[superBlock.block_size];
	long chInode = depthMinix3(path, &di);

	if(chInode == -1) return -1;
	if(di.parentInode == 0) return -1;

	struct InodeMinix3 inode = getInodeMinix3(chInode);
	
	if((inode.mode & S_IFMT) == S_IFDIR) return -1;

	if(deleteDirMinix3(di) == -1) return -1;

	--inode.nlinks;
	putInodeMinix3(chInode, inode);

	if(inode.nlinks == 0) {
		deleteZonesMinix3(inode, (uint32_t*)buffer);

		deallocImapMinix3(chInode);
		struct InodeMinix3 tmp = {0};
    	putInodeMinix3(chInode, tmp);
	}

	return 0;
}

long creatFileMinix3(const char* name, long parentInode, uint16_t mode) {
    uint32_t inodeNum = allocImapMinix3();
    if(inodeNum == 0) return -1;

    struct InodeMinix3 inode = {0};
	inode.nlinks = ((mode & S_IFMT) == S_IFDIR) ? 2 : 1;
    inode.mode = mode;
	putInodeMinix3(inodeNum, inode);

	return (long)inodeNum;
}

uint64_t readInodeMinix3(long inode_num, uint8_t* buff, uint64_t size, uint64_t offset) {
	if(inode_num == -1) return 0;
	
	uint64_t bytesRead = 0, blockOffset = offset % superBlock.block_size;
	uint8_t buffer[superBlock.block_size];
	struct InodeMinix3 inode = getInodeMinix3(inode_num);

	if(offset >= inode.size) return 0;

	uint64_t ssize = (size > (inode.size - offset)) ? (inode.size - offset) : size;

	switch(inode.mode & S_IFMT) {
		case S_IFREG:
			for(int64_t blk = offset / superBlock.block_size; bytesRead < ssize && blk < superBlock.zones; ++blk) {
				uint64_t curRead = ((ssize - bytesRead) > (superBlock.block_size - blockOffset)) ? (superBlock.block_size - blockOffset) : (ssize - bytesRead);
				uint32_t idx = processingZoneMinix3(inode, blk, (uint32_t*)buffer);
				
				if(idx == 0) {
					memset((void*)(buff + bytesRead), 0, curRead);
					bytesRead += curRead;
					continue;
				}
				
				readSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)idx * offsetBlock, (void*)buffer, superBlock.block_size);
				
				memcpy((void*)(buff + bytesRead), (void*)(buffer + blockOffset), curRead);
				
				bytesRead += curRead;
				blockOffset = 0;
			}
			break;
		case S_IFDIR:
			break;
	};

	return bytesRead;
}

uint64_t writeInodeMinix3(long inode_num, uint8_t* buff, uint64_t size, uint64_t offset) {
	if(inode_num == -1) return 0;

	uint64_t bytesWrite = 0, blockOffset = offset % superBlock.block_size;
	uint8_t buffer[superBlock.block_size];
	struct InodeMinix3 inode = getInodeMinix3(inode_num);

	switch(inode.mode & S_IFMT) {
		case S_IFREG:
			for(int64_t blk = offset / superBlock.block_size; blk < superBlock.zones && bytesWrite < size; ++blk) {
				uint64_t curWrite = ((size - bytesWrite) > (superBlock.block_size - blockOffset)) ? (superBlock.block_size - blockOffset) : (size - bytesWrite);
				uint32_t idx = ensureZoneMinix3(&inode, blk, (uint32_t*)buffer);
				if(idx == 0) return bytesWrite;
				
				readSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)idx * offsetBlock, (void*)buffer, superBlock.block_size);

				memcpy((void*)(buffer + blockOffset), (void*)(buff + bytesWrite), curWrite);
				
				writeSDMMC(globalDTB.sd.addr, NONDMA, LBA + (uint64_t)idx * offsetBlock, (void*)buffer, superBlock.block_size);
				
				bytesWrite += curWrite;
				blockOffset = 0;
			}
			break;
		case S_IFDIR:
			break;
	};

	inode.size = (bytesWrite + offset > inode.size) ? bytesWrite + offset : inode.size;

	putInodeMinix3(inode_num, inode);

	return bytesWrite;
}
