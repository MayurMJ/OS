#include <syscall.h>

int open(const char *fileName, int mode) {
  return syscall2(2, (long) fileName, mode);
}
