#include <unistd.h>
#include <stdlib.h>
#include <sys/defs.h>

int getsysid() {
	int a = 3;
	int b = 8;
	b = b*a*a;
	return b;
}
//int result;
int main(int argc, char *argv[], char *envp[]) {
	int result  = 1;
	int n;
	int syscallno;
	result = fork();
	if (result == 0) {
//		__asm__ __volatile__("pushq %rcx\n\t");
//		__asm__ __volatile__("popq %rcx\n\t");
		syscallno = 11;
 	        __asm__ __volatile__("int $0x80\n\t"	//a sycall that simply prints I'm in child
                                     :"=a" (n)
                                     : "0"(syscallno));
		__asm__ __volatile__("int $0x80\n\t"
                             	     :"=a" (n)
                                     : "0"(24));
		syscallno = 24;
                __asm__ __volatile__("int $0x80\n\t"
                                     :"=a" (n)
                                     : "0"(syscallno));
		}
//		int b = getsysid();
//		__asm__ __volatile__("int $0x80\n\t"    //a sycall that simply prints I'm in child
//                                     :"=a" (n)
//                                     : "0"(b));			
	} 
	else {
		wait(); //parent waits for the child to finish
		syscallno = 10;
		__asm__ __volatile__("int $0x80\n\t"    //a sycall that simply prints I'm in child
                                     :"=a" (n)
                                     : "0"(syscallno));
	}
	                __asm__ __volatile__("int $0x80\n\t"
                                     :"=a" (n)
                                     : "0"(result));
	//while(1); //no need to return from bin/init
	return 0;
}
