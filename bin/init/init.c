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

//ERROR: trying to allocate a non avaialable page   
#if 0
int main(int argc, char *argv[], char *envp[]) {
		printf("\nParent");
		int a[100000];
		for(int i = 0;i<100000;i++) {
			a[i] = i%1000;
			printf("%d",a[i]+(getsysid()));
		}
		printf("\nParent");
//	while(1); //no need to return from bin/init
	return 0;
}
#endif

int main(int argc, char *argv[], char *envp[]) {
		fork();
		fork();
		fork();
		fork();

//      while(1); //no need to return from bin/init
        return 0;
}


