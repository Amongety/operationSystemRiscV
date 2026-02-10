#ifndef __HTRAP_H__
#define __HTRAP_H__

#include "common.h"
#include "../../libsbi/sci.h"
#include "../../process/process_control.h"
#include "systemCalls/syscall.h"

extern struct Process bufProc[PROCESS_MAX];
extern struct Process* prevProcess;

uint32_t handler_trap(struct trap_frame* t);

#endif
