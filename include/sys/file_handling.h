#ifndef _FILE_HANDLING_H
#define _FILE_HANDLING_H
#include <sys/initfs.h>
int allocate_file_object(dentry *file_entry);
void deallocate_file_object(int fd);
#endif
