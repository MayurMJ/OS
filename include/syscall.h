#ifndef _UNISTD_H
#define _UNISTD_H

#include <sys/defs.h>

static inline long syscall4(long syscall, long arg1, long arg2, long arg3, long arg4) {
long ret;
__asm volatile ("syscall" : "=a" (ret) : "a" (syscall), "b" (arg1), "c" (arg2),
 "d" (arg3), "e" (arg4) : "memory");
return ret;
}

static inline long syscall3(long syscall, long arg1, long arg2, long arg3) {
long ret;
__asm volatile ("syscall" : "=a" (ret) : "a" (syscall), "b" (arg1), "c" (arg2),
 "d" (arg3) : "memory");
return ret;
}

static inline long syscall2(long syscall, long arg1, long arg2) {
long ret;
__asm volatile ("syscall" : "=a" (ret) : "a" (syscall), "b" (arg1), "c" (arg2) : "memory");
return ret;
}

static inline long syscall1(long syscall, long arg1) {
long ret;
__asm volatile ("syscall" : "=a" (ret) : "a" (syscall), "b" (arg1) : "memory");
return ret;
}

static inline long syscall0(long syscall, long arg1) {
long ret;
__asm volatile ("syscall" : "=a" (ret) : "a" (syscall) : "memory");
return ret;
}

#endif
