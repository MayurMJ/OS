#include <unistd.h>
#include <stdlib.h>
#include <sys/defs.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]) {
        char fileName[100];
        char input[100];
	memset((void*)fileName ,0 ,100);
	memset((void*)input ,0 ,100);
	getcwd(fileName, 100);
	if(argc == 1) {
		strcpy(input, fileName);
	}
	else {
		if(argv[1][0] == '/') {
			strcpy(input, argv[1]);
		}
		else {
			strcat(input, fileName);
			strcat(input, argv[1]);
		}
	}
	//puts(input);
	//puts("\n");
        DIR *fd = opendir(input);
	if(fd) {
        	ldirent *ld = readdir(fd);
        	while(ld != NULL) {
                	puts(ld->d_name);
			puts("\n");
               		free(ld);
                	ld = readdir(fd);
        	}
	}
	else {
		printf("ls: cannot access '%s': No such file or directory", input);
	}
	return 0;
}

