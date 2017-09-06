#include <syscall.h>
pid_t fork(void) {
  return syscall0(57);
}
