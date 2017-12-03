#include<sys/defs.h>
#include<dirent.h>
int opendir(const char *fileName) {
    int retval;
    uint64_t code = 78;
    __asm__ __volatile("int $0x80\n\t"
                       :"=a"(retval)
                       :"a"(code), "D"((uint64_t)fileName)
		       :"memory");
    return retval;
}

int closedir(int fd) {
    uint64_t retval;
    uint64_t code = 76;
    __asm__ __volatile("int $0x80\n\t"
                       :"=a"(retval)
                       :"a"(code), "D"((uint64_t)fd)
		       :"memory");
    return retval;
}

DIR* readdir(int fd) {
    uint64_t retval;
    uint64_t code = 77;
    __asm__ __volatile("int $0x80\n\t"
                       :"=a"(retval)
                       :"a"(code), "D"((uint64_t)fd)
		       :"memory");
    return (DIR*)retval;
}
