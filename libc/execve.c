#include <syscall.h>

int execve(const char *file, char *const argv[], char *const envp[]) {
  return (syscall3(59,(long)file,(long)argv,(long)envp));
}

