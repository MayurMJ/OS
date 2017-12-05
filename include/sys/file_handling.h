#ifndef _FILE_HANDLING_H
#define _FILE_HANDLING_H
#include <sys/initfs.h>

typedef struct linux_dirent {
  unsigned long  d_ino;     /* Inode number */
  unsigned long  d_off;     /* Offset to next linux_dirent */
  unsigned short d_reclen;  /* Length of this linux_dirent */
  char           d_name[];  /* Filename (null-terminated) */
           } ldirent;

int allocate_file_object(dentry *file_entry);
void deallocate_file_object(int fd);
int allocate_new_dir(dentry *file_entry);
void deallocate_new_dir(int fd);
#endif
