#ifndef __FS_H__
#define __FS_H__

#include <stddef.h>
#include "parsingDisk.h"
#include "../libc/string.h"

#define LEN_NAME_DIR_ENTRY 60

#define MAGIC_MINIX_3 0x4d5a

#define S_IFMT 0170000		/* маска тип файла */

#define S_IFLNK 0120000		/* символьная ссылка */
#define S_IFREG 0100000		/* файл */
#define S_IFBLK 0060000		/* блочное устройство */
#define S_IFDIR 0040000		/* директория */
#define S_IFCHR 0020000		/* символьное устройство */
#define S_IFIFO 0010000		/* фифо */

struct SuperBlockMinix3 {
	uint32_t ninodes;		/* Общее количество inode */
	uint16_t pad0;
	uint16_t imap_blocks; 		/* Количество блоков битовой карты inode */
	uint16_t zmap_blocks;		/* Количество блоков битовой карты зон (блоков) */
	uint16_t first_data_zone;	/* Первая зона данный */
	uint16_t log_zone_size;		/* Количество блоков в зоне */
	uint16_t pad1;
	uint32_t max_size;		/* Максимальный размер файла */
	uint32_t zones;			/* Общее количество зон */
	uint16_t magic;
	uint16_t pad2;
	uint16_t block_size;		/* Размер одного блока */
	uint8_t disk_version;		/* Подверсия ФС */
};

struct InodeMinix3 {
	uint16_t mode;			/* Права и тип файлов */
	uint16_t nlinks;		/* Жесткая ссылка */
	uint16_t uid;			/* User id */
	uint16_t gid;			/* Group id */
	uint32_t size;			/* Размер в байтах */
	uint32_t attime;		/* Время последнего доступа */
	uint32_t mtime;			/* Время последнего изменения */
	uint32_t ctime;			/* Время последнего изменения метаданных */
	uint32_t zone[10];		/* Указатель на блоки данных в диске */
};

struct DirEntryMinix3 {
	uint32_t inode;
	char name[LEN_NAME_DIR_ENTRY];
};

struct DirInfo {
	int indexDir;
	long parentInode;
	uint64_t sector;
	char name[LEN_NAME_DIR_ENTRY];
};

static struct SuperBlockMinix3 superBlock;
static uint32_t LBA;
static uint8_t offsetBlock;

uint32_t getInodeSize(long inode_num);
void fsInitMinix3(void);
struct InodeMinix3 getInodeMinix3(long inode_num);
void putInodeMinix3(long inode_num, struct InodeMinix3 src);
uint32_t processingZoneMinix3(struct InodeMinix3 inode, long logBlck, uint32_t* buff);
uint32_t ensureZoneMinix3(struct InodeMinix3 *inode, long logBlck, uint32_t* buff);
void deleteZonesMinix3(struct InodeMinix3 inode, uint32_t* buff);
uint32_t allocZoneMinix3(void);
int deallocZmapMinix3(uint32_t zone);
uint32_t allocImapMinix3(void);
int deallocImapMinix3(long inode_num);
int allocDirMinix3(uint32_t chInode, uint32_t pInode, const char* name);
long searchPath(struct DirInfo* di);
long depthMinix3(const char* path, struct DirInfo* di);
long openMinix3(const char* path, uint16_t mode);
long mkdirMinix3(const char* path, uint16_t mode);
int rmdirMinix3(const char* path);
int deleteDirMinix3(struct DirInfo di);
int unlinkMinix3(const char* path);
long creatFileMinix3(const char* name, long parentInode, uint16_t mode);
uint64_t readInodeMinix3(long inode_num, uint8_t* buff, uint64_t size, uint64_t offset);
uint64_t writeInodeMinix3(long inode_num, uint8_t* buff, uint64_t size, uint64_t offset);

#endif
