#include <syscall.h>

int dup2(int fildes, int fildes2) {
  return (syscall2(33,(long) fildes, (long) fildes2 ));
}
