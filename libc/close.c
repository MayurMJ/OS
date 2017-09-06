#include <syscall.h>

int close(const char *fileName) {
  return syscall1(3, (long) fileName);
}
