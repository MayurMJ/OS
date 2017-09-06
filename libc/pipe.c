#include<syscall.h>

int pipe(int pipefd[2]) {
 return (syscall1(22,(long)pipefd));
}
