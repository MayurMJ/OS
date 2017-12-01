#include <unistd.h>
#include <stdlib.h>
#include <sys/defs.h>

int getsysid() {
	int a = 3;
	int b = 8;
	b = b*a;
	return b;
}
//int result;
int main(int argc, char *argv[], char *envp[]) {
	int result  = 1;
	int n;
	int syscallno;
	result = fork();
	if (result == 0) {
		syscallno = 11;
 	        __asm__ __volatile__("int $0x80\n\t"	//a sycall that simply prints I'm in child
                                     :"=a" (n)
                                     : "0"(syscallno));
/*		syscallno = 24;
 		__asm__ __volatile__("int $0x80\n\t"
                                     :"=a" (n)
                                     : "0"(syscallno));
*/		syscallno = 57;
		__asm__ __volatile__("int $0x80\n\t"
                             	     :"=a" (n)
                                     : "0"(syscallno));
/*		syscallno = 24;
                __asm__ __volatile__("int $0x80\n\t"
                                     :"=a" (n)
                                     : "0"(syscallno));

		if (n == 0) {
			syscallno = 12;
                	__asm__ __volatile__("int $0x80\n\t"    //a sycall that simply prints I'm in child
                                             :"=a" (n)
                                             : "0"(syscallno));
		}
*/	} 
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
	while(1); //no need to return from bin/init
	return 0;
}
