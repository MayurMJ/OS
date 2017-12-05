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
                             : "0"(syscallno), "D"(10));
	puts("sleep over\n");
	return 0;
}

