#include<sys/defs.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/kernel_threads.h>
#include<string.h>
DIR* opendir(const char *fileName) {
    DIR* ret_dir = (DIR*)malloc(sizeof(DIR)); 
    int retval;
    uint64_t code = 78;
    __asm__ __volatile("int $0x80\n\t"
                       :"=a"(retval)
                       :"a"(code), "D"((uint64_t)fileName)
		       :"memory");
    ret_dir->fd = retval;
    return ret_dir;
}

int closedir(DIR *fd) {
    uint64_t retval;
    uint64_t code = 76;
    __asm__ __volatile("int $0x80\n\t"
                       :"=a"(retval)
                       :"a"(code), "D"((uint64_t)fd->fd)
		       :"memory");
    free(fd);
    return retval;
}

ldirent* readdir(DIR* fd) {
    ldirent* ret_dir = (ldirent*) malloc(sizeof(ldirent));
    uint64_t retval;
    uint64_t code = 77;
    __asm__ __volatile("int $0x80\n\t"
                       :"=a"(retval)
                       :"a"(code), "D"((uint64_t)fd->fd), "S"((uint64_t)ret_dir)
		       :"memory");
    ldirent *temp_ret = (ldirent*) retval;
    if(strcmp(temp_ret->d_name, "") == 0)
	return NULL;
    else
	return temp_ret;
}
