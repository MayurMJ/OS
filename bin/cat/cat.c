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
                puts("please Enter a fileName");
	}
	else {
		if(argv[1][0] == '/') {
			strcpy(input, argv[1]);
		}
		else {
			strcat(input, fileName);
			strcat(input, argv[1]);
		}
        	int fd = open(input, 0);
		//printf("trying to find %s\n",input);
        	//FILE *fp = fopen(fileName, "r");
		if (fd == -1) {
			puts(input);
			puts(" not found"); putchar('\n');
			return 0;
		}
        	char ch;
       		while ((ch = getc(fd)) != EOF) {
               		putchar(ch);
        	}
	}
	return 0;
}

