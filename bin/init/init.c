#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {
	//int result  = fork();
	fork();
/*	if (result == 0) {
		execve("bin/sbush", NULL, NULL);
	} */
	while(1); //no need to return from bin/init
}
