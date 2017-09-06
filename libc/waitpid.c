#include <syscall.h>

int waitpid(int pid, int *status) {
  return syscall4(61, pid, (long) status, 0, 0);
}

