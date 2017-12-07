#include <unistd.h>
#include <stdlib.h>
#include <sys/defs.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]) {	
	if(argc == 1) {
		printf("\n Usage: Sleep <time>(in miliseconds)");
	}
	else {
		int wait_time = atoi(argv[1]);
		uint64_t syscallno = 35;
	    	uint64_t start_timer;
		puts("starting sleep\n");
    		__asm__ __volatile__("int $0x80\n\t"
                	             :"=a" (start_timer)
                        	     : "0"(syscallno), "D"(wait_time));
		puts("sleep over\n");
	}
	return 0;
}

