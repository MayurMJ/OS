#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {
	int result  = 1;
	int yield = 24;
	int n;
	//fork();
	result = fork();
	if (result == 0) {
		//execve("bin/sbush", NULL, NULL);
		
 	        __asm__ __volatile__("int $0x80\n\t"	//a sycall that simply prints I'm in child
                              :"=a" (n)
                              : "0"(11));
 //		yield = getsysid();
 		        __asm__ __volatile__("int $0x80\n\t"
                               :"=a" (result)
                               : "0"(yield));
	} 
	                        __asm__ __volatile__("int $0x80\n\t"
                               :"=a" (result)
                               : "0"(yield));
		                __asm__ __volatile__("int $0x80\n\t"    //a sycall that simply prints I'm in child
                              :"=a" (n)
                              : "0"(10));
	while(1); //no need to return from bin/init
}
