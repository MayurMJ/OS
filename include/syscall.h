#ifndef _UNISTD_H
#define _UNISTD_H

#include <sys/defs.h>

static inline long syscall3(long syscall, long arg1, long arg2, long arg3) {
long ret;
__asm volatile ("int $0x80" : "=a" (ret) : "a" (syscall), "b" (arg1), "c" (arg2),
 "d" (arg3) : "memory");
return ret;
}

#endif
