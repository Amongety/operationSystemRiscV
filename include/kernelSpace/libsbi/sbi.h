#ifndef __SBI_H__
#define __SBI_H__

#include "../config.h"

struct SbiRet {
	long error;
	union {
		long value;
		unsigned long uvalue;
	};
};

#ifdef __DBCN__
	typedef struct SbiRet console_ret_t;
#else
	typedef int console_ret_t;
#endif

#endif
