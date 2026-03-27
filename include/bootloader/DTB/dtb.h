#ifndef __DTB_H__
#define __DTB_H__

#include <stdint.h>
#include <stdbool.h>
#include "../../kernelSpace/libsbi/le.h"
#include "../../kernelSpace/libc/byteOrder.h"
#include "../../kernelSpace/libc/string.h"
#include "platformDevices.h"

struct dtbPlatform dtbPlt;

struct fdt_header {
	uint32_t magic;
	uint32_t totalsize;
	uint32_t off_dt_struct;
	uint32_t off_dt_strings;
	uint32_t off_mem_rsvmap;
	uint32_t version;
	uint32_t last_comp_version;
	uint32_t bool_cpuid_phys;
	uint32_t size_dt_strings;
	uint32_t size_dt_struct;
};

#define FDT_BEGIN_NODE 0x00000001
#define FDT_END_NODE 0x00000002
#define FDT_PROP 0x00000003
#define FDT_NOP 0x00000004
#define FDT_END 0x00000009

struct fdt_mmio {
	uint64_t address;
	uint64_t size;
};

struct fdt_prop {
	uint32_t len;
	uint32_t nameoff;
};

#endif
