#include <unistd.h>
#include <stdlib.h>
#include <sys/defs.h>
#include <stdio.h>

int getsysid() {
	int a = 3;
	int b = 8;
	b = b*a*a;
	return b;
}
//int result;
int main(int argc, char *argv[], char *envp[]) {
/*	int result  = 1;
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
		syscallno = 10;
		__asm__ __volatile__("int $0x80\n\t"    //a sycall that simply prints I'm in child
                                     :"=a" (n)
                                     : "0"(syscallno));
		syscallno = 24;
	        __asm__ __volatile__("int $0x80\n\t"
                                     :"=a" (n)
                                     : "0"(syscallno));
	}
*/
	//ssize_t retval;
	//int count = 100;
	int n;
	int x = open("/rootfs/bin/sbush", 3);
	                __asm__ __volatile__("int $0x80\n\t"
                                     :"=a" (n)
                                     : "0"(x + 100));
	close(x);

        //uint64_t buffer[512];
	/*for(int x=0;x<10;x++) {
        uint64_t buffer[512];
        __asm__ __volatile("int $0x80\n\t"
                           :"=a"(retval)
                           :"a"(0),"D"(0),"S"(buffer),"d"(count));
	__asm__ __volatile("int $0x80\n\t"
			   :"=a"(retval)
			   :"a"(59), "D"((uint64_t)buffer), "S"((uint64_t)0), "d"((uint64_t)0)
			   :"memory");
	}*/
	char *strtemp = "gargi";
	int retval;
	__asm__ __volatile("int $0x80\n\t"
                           :"=a"(retval)
                           :"a"(1),"D"(1),"S"((uint64_t)strtemp),"d"(0));
	while(1); //no need to return from bin/init
	return 0;
}
