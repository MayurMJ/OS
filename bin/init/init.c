#include <unistd.h>
#include <stdlib.h>
#include <sys/defs.h>
#include <stdio.h>
#include <dirent.h>
#include <signal.h>

int getsysid() {
	int a = 3;
	int b = 8;
	b = b*a*a;
	return b;
}
//int result;
int main(int argc, char *argv[], char *envp[]) {
	int res = fork();
        if(res == 0) {
		printf("Child");
        }
        else {
		printf("\nParent");
        }
	
//	while(1); //no need to return from bin/init
	return 0;
}
