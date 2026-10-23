#include "../../include/userSpace/sys/ioctl.h"

void ioctl(uint64_t device, uint64_t op, void* arg) {
    register uint64_t a0 asm("a0") = device;
    register uint64_t a1 asm("a1") = op;
    register uint64_t a2 asm("a2") = (uint64_t)arg;
    register uint64_t a7 asm("a7") = SYSCALL_IOCTL;

    asm volatile(
        "ecall"
        :
        : "r"(a0),
          "r"(a1),
          "r"(a2),
          "r"(a7)
        : "memory"
    );
}