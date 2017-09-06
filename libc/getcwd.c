#include <syscall.h>

char* getcwd(char *buf, size_t size) {
  return (char *)syscall2(79, (long) buf, size);
}
