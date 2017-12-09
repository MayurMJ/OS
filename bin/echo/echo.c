#include <stdio.h>
#include <sys/defs.h>

int main(int argc, char *argv[], char *envp[]) {	
	if(argc == 1) {
               // puts("please Enter a fileName");
        }
	else {
		int i =1;
		while(argv[i] != NULL) {
			if (argv[i][0] != '\0') {
				printf("%s ", argv[i]);
			}
			i++;
		}
		printf("\n");
        }
	return 0;
}

