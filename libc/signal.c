#include <signal.h>

int kill(pid_t pid, int sig) {
    if(sig != 9) {
	return -1;
    }
    uint64_t retval;
    uint64_t code = 62;
    __asm__ __volatile("int $0x80\n\t"
                       :"=a"(retval)
                       :"a"(code), "D"((uint64_t)pid), "S" ((uint64_t) sig)
                       :"memory");
    return (int)retval;


}
