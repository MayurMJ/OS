#include <sys/defs.h>



pid_t fork() {
    uint64_t syscallno = 57;
    uint64_t result = 1;
    __asm__ __volatile__("int $0x80\n\t"
                             :"=a" (result)
                             : "0"(syscallno));
    return result;
}

pid_t yield() {
    uint64_t syscallno = 24;
    uint64_t result = 1;
    __asm__ __volatile__("int $0x80\n\t"
                             :"=a" (result)
                             : "0"(syscallno));
    return result;
}
uint64_t getsysid() {
    uint64_t syscallno = 3;
    uint64_t result = 8;
    result = result *syscallno;
    return result;
    
}

        int i = 0;
int main(int argc, char *argv[], char *envp[]) {
        int result=1; 
	int n = 57; 
	int yield =24;

	result = fork();
	if(result==0) {
	     
	        __asm__ __volatile__("int $0x80\n\t"	//a sycall that simply prints I'm in child
                             :"=a" (n)
                             : "0"(11));
	//		yield = getsysid();
		        __asm__ __volatile__("int $0x80\n\t"
                             :"=a" (result)
                             : "0"(yield));
	    
	} else {
	    __asm__ __volatile__("int $0x80\n\t"	//a sycall that simply prints I'm in parent
                             :"=a" (n)
                             : "0"(10));
	//		yield = getsysid();
	            __asm__ __volatile__("int $0x80\n\t"
                             :"=a" (result)
                             : "0"(yield));
	}
        while(1);
  return 0;
}
