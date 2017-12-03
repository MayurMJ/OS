#ifndef _FILE_HANDLING_H
#define _FILE_HANDLING_H
#include <sys/initfs.h>


int allocate_file_object(dentry *file_entry);
void deallocate_file_object(int fd);
int allocate_new_dir(dentry *file_entry);
void deallocate_new_dir(int fd);
#endif
