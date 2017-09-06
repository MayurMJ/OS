#include <syscall.h>

int chdir(const char *path) {
  return syscall1(80, (long) path);
}
