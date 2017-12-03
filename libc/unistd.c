#include <sys/defs.h>

pid_t fork() {
    uint64_t syscallno = 57;
    uint64_t result = 1;
    __asm__ __volatile__("int $0x80\n\t"
                             :"=a" (result)
                             : "0"(syscallno));
    return result;
}

int execve(const char *file, char *const argv[], char *const envp[]) {
    int retval;
    uint64_t code = 59;
    __asm__ __volatile("int $0x80\n\t"
		       :"=a"(retval)
		       :"a"(code), "D"((uint64_t)file), "S"((uint64_t)argv), "d"((uint64_t)envp)
		       :"memory");
    return retval;
}

ssize_t read(int fd, void *buf, size_t count) {
    ssize_t retval;
    uint64_t code = 0;
    uint64_t buffer = (uint64_t)(char *)buf;
    __asm__ __volatile("int $0x80\n\t"
                       :"=a"(retval)
                       :"a"(code),"D"(fd),"S"(buffer),"d"(count));
    return (ssize_t)retval;
}

pid_t wait(int *status) {
    int retval;
    uint64_t code = 61;
    __asm__ __volatile("int $0x80\n\t"
                       :"=a"(retval)
                       :"a"(code), "D"((uint64_t)status));
    return retval;
}

int open(const char *fileName, int mode) {
    int retval;
    uint64_t code = 5;
    __asm__ __volatile("int $0x80\n\t"
                       :"=a"(retval)
                       :"a"(code), "D"((uint64_t)fileName), "S"((uint64_t) mode)
		       :"memory");
    return retval;

}

