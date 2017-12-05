#include <unistd.h>
#include <stdlib.h>
#include <sys/defs.h>
#include <stdio.h>
#include <dirent.h>

int atoi(char *s) {
	return 0;	
}

int main(int argc, char *argv[], char *envp[]) {	
	uint64_t syscallno = 35;
    	uint64_t start_timer;
	puts("starting sleep\n");
    	__asm__ __volatile__("int $0x80\n\t"
                             :"=a" (start_timer)
                             : "0"(syscallno));
/*	puts("starting sleep\n");
	//int curr_timer;
	while (1) {
		puts("waiting");
		volatile int curr_timer;
		__asm__ __volatile__("int $0x80\n\t"
                             :"=a" (curr_timer)
                             : "0"(syscallno));
		if (curr_timer - start_timer < 5) {continue;}
		else break;
	}
*/	puts("sleep over\n");
	return 0;
}

