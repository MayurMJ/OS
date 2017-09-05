#include<syscall.h>


size_t read(int fd, const void* buf, size_t count) {
  return syscall3(0, fd,(long) buf, count);
}

