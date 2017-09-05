#include <sys/defs.h>
#include <syscall.h>

//static inline long syscall3(long syscall, long arg1, long arg2, long arg3) {
//long ret;
//__asm volatile ("int $0x80" : "=a" (ret) : "a" (syscall), "b" (arg1), "c" (arg2),
// "d" (arg3) : "memory");
//return ret;
//}


size_t write(int fd, const void* buf, size_t count) {
  return syscall3(4, fd,(long) buf, count);
}
