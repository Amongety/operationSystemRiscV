#ifndef __SBI_H__
#define __SBI_H__

#include "../config.h"

struct sbiret {
	long error;
	union {
		long value;
		unsigned long uvalue;
	};
};

#ifdef __DBCN__
	typedef struct sbiret console_ret_t;
#else
	typedef int console_ret_t;
#endif

#endif
