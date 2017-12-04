#include <unistd.h>
#include <stdlib.h>
#include <sys/defs.h>
#include <stdio.h>
#include <dirent.h>

int main(int argc, char *argv[], char *envp[]) {	
	if(argc == 1) {
                puts("please Enter a fileName");
        }
	else {
        	char *fileName = argv[1];
        	int fd = open(fileName, 3);
        	//FILE *fp = fopen(fileName, "r");
		if (fd == -1) {
			puts(fileName);
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

