#include <unistd.h>
#include <stdlib.h>
#include <sys/defs.h>

int getsysid() {
	int a = 3;
	int b = 8;
	b = b*a;
	return b;
}
int result;
int n;
int main(int argc, char *argv[], char *envp[]) {
/*	int result  = 1;
	int yield = 24;
	int n;
	//fork();
	result = fork();
	if (result == 0) {
		//execve("bin/sbush", NULL, NULL);
		
	int yield;
	yield = 24;
	for(int i =0; i <5; i++) {
 	        __asm__ __volatile__("int $0x80\n\t"	//a sycall that simply prints I'm in child
                              :"=a" (n)
                              : "0"(11));
 		yield = getsysid();
 		        __asm__ __volatile__("int $0x80\n\t"
                               :"=a" (result)
                               : "0"(yield));
	}
	} 
	else {
		int yield;
		yield = 24;
		                __asm__ __volatile__("int $0x80\n\t"    //a sycall that simply prints I'm in child
                              :"=a" (n)
                              : "0"(10));
	                        __asm__ __volatile__("int $0x80\n\t"
                               :"=a" (result)
                               : "0"(yield));
	}
*/
	if (argc == 3) {		
	                __asm__ __volatile__("int $0x80\n\t"    //a sycall that simply prints I'm in child
                              :"=a" (n)
                              : "0"(11), "D"(argc));
	}
	while(1); //no need to return from bin/init
}
