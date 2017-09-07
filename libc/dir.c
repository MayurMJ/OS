#include <unistd.h>
#include <syscall.h>
#include <dirent.h>
#include <stdio.h>

#define MAX_DIR 10

 DIR dirs[MAX_DIR];
int next_free_dir = 0;


DIR * opendir(const char *name) {
  int fd;
  if(next_free_dir == MAX_DIR)
    return 0;
  DIR * dir = &dirs[next_free_dir++];
  fd = open(name,0);
  if((fd<0))
    return 0;
  dir->fd = fd;
  return dir;
}
ldirent * readdir(DIR *dirp) {
  ldirent * ld;
  int len = syscall3(SYS_getdents,(long) dirp->fd,(long) dirp->buff,(long) (sizeof (dirp->buff)));
  if (len == -1)
  {
    puts("syscall SYS_getdents failure!\n");
    return 0;
  }
  if (len == 0)
  {
    return 0;
  }
  ld = (ldirent *) dirp->buff;
  return ld;
}
