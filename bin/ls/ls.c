#include <unistd.h>
#include <stdlib.h>
#include <sys/defs.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]) {	
        char fileName[100];
	if(argc == 1) {
		getcwd(fileName, 100);
	}
	else {
		strcpy(fileName, argv[1]);
	}	
        DIR *fd = opendir(fileName);
        ldirent *ld = readdir(fd);
        while(ld != NULL) {
                puts(ld->d_name);
                free(ld);
                ld = readdir(fd);
        }
	return 0;
}

