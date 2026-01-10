/* sbi console interface */

#ifndef __SCI_H__
#define __SCI_H__

#include "../config.h"
#include <stdarg.h>
#include "le.h"
#include "dbcn.h"

sbi_ret_t sbi_console_printf(const char* format, ...);

#endif
