#ifndef _DIRENT_H
#define _DIRENT_H
#include <sys/initfs.h>
#define NAME_MAX 255
#define SYS_getdents 78
struct dirent {
 char d_name[NAME_MAX+1];
};
typedef struct linux_dirent {
  unsigned long  d_ino;     /* Inode number */
  unsigned long  d_off;     /* Offset to next linux_dirent */
  unsigned short d_reclen;  /* Length of this linux_dirent */
  char           d_name[];  /* Filename (null-terminated) */
           } ldirent;

struct DIR {
  int fd;
  char buff[100];
};

typedef struct DIR DIR;


DIR *opendir(const char *name);
ldirent *readdir(DIR *dirp);
int closedir(DIR *dirp);


#endif
