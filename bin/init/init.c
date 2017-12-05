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

/*	int res = fork();
        if(res == 0) {
		printf("Child");
        }
        else {
		printf("\nParent");
                printf("Im in child\n");
		char *temp = (char *)malloc(10);
		printf("allocated temp\n");
		free(temp);
		printf("deallocated temp\n");
                //char *binary = "bin/sbush";
                //execve(binary, NULL, NULL);
                
                //execve(binary, NULL, NULL);           
        }
        else {
		printf("Im in parent\n");
                //int status;
                //wait(&status);
        }
*/
/*	int res = fork();
	if (res == 0) {
		char *binary = "bin/sbush";
		execve(binary, NULL, NULL);
	}
	else {
		int status;
		wait(&status);
	}
*/
/*	uint64_t temp_block = ret_brk();
	printf("curr brk point %x\n",temp_block);
	int ret = brk((void *)(0xc0000000-0x10000000-4096));
	printf("brk returned %d\n",ret);
	ret = brk((void *)(0xc0000000-0x10000000-8192));
	printf("brk returned %d\n",ret);
	ret = brk((void *)(0xc0000000-0x10000000));
        printf("brk returned %d\n",ret);
*/
//	while(1); //no need to return from bin/init
	return 0;
}
