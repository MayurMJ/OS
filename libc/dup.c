#include <syscall.h>

int dup(int fildes) {
  return (syscall1(32,(long) fildes));
}
