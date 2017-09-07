#include <syscall.h>
#include <unistd.h>

int getdents(unsigned int fd,  ldirent *dirp,unsigned int count) {
 	return syscall3(78, (long) fd, (long)dirp,(long)count);

}
