#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {	
	if(argc == 1) {
               // puts("please Enter a fileName");
        }
	else {
		for(int i = 0; i < argc; i++) {
			printf("\n%s", argv[i]);
		}
        }
	return 0;
}

