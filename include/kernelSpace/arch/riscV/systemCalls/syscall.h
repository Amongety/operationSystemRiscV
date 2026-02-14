#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "../../../process/process_control.h"
#include "../common.h"

#define SYSCALL_CLONE 					0xdc
#define SYSCALL_EXIT 					0x5d
#define SYSCALL_WAITPID 				0x5f
#define SYSCALL_EXECVE 					0xdd
#define SYSCALL_GETPID 					0xac
#define SYSCALL_OPENAT 					0x38
#define SYSCALL_CLOSE 					0x39
#define SYSCALL_READ 					0x3f
#define SYSCALL_WRITE 					0x40
#define SYSCALL_LSEEK 					0x3e
#define SYSCALL_UNLINKAT 				0x23
#define SYSCALL_RENAMEAT2 				0x114
#define SYSCALL_MKDIRAT 				0x22
#define SYSCALL__						'?'
#define SYSCALL_CHDIR 					0x31
#define SYSCALL_STATFS 					0x2b
#define SYSCALL_FSTATFS 				0x2c
#define SYSCALL_LINKAT 					0x25
#define SYSCALL_SYMLINKAT 				0x24
#define SYSCALL_BRK 					0xd6
#define SYSCALL_MMAP 					0xde
#define SYSCALL_CLOCK_GETTIME 				0x71
#define SYSCALL_						 '?'
#define SYSCALL_GETUID 					0xae
#define SYSCALL_GETGID 					0xb0
#define SYSCALL_SOCKET 					0xc6
#define SYSCALL_BIND 					0xc8
#define SYSCALL_LISTEN 					0xc9
#define SYSCALL_ACCEPT 					0xca
#define SYSCALL_CONNECT 				0xcb
#define SYSCALL_SENDTO 					0xce
#define SYSCALL_RECVFROM 				0xcf
#define SYSCALL_UMASK	 				0xa6
#define SYSCALL_FCHOWN 					0x37

extern struct Process bufProc[PROCESS_MAX];
extern struct Process* prevProcess;

uint64_t do_syscall(struct trap_frame* tf);
void do_exit(void);

#endif
