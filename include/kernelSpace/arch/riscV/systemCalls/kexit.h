#ifndef __KEXIT_H__
#define __KEXIT_H__

#include "../process/process_control.h"

extern struct Process bufProc[PROCESS_MAX];
extern struct Process* prevProcess;

void do_exit(void);

#endif