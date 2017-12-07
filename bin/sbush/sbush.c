#include <unistd.h>
#include <stdlib.h>
#include <sys/defs.h>
#include <stdio.h>
#include <string.h>
#define TOKENSIZE 100

int cd(char **args, char *envp[]);
int export(char **args, char *envp[]);
int sbushExit(char **args, char *envp[]);

char *builtInCommands[] = {
	"cd",
	"export",
	"exit"
};

int (*builtInFunc[]) (char **args, char *envp[]) = {
	&cd,
	&export,
	&sbushExit
};

int cd(char **args, char *envp[])
{
	if (args[1] != NULL) {
		if (chdir(args[1]) == -1) {
			printf("sbush> Directory not found\n");
			return 0;
    		}
		else {
			printf("sbush> Directory changed to %s\n",args[1]);
		}
  	}
	else {
		printf("sbush> Missing argument to cd");
		return 0;
	}
  	return 0;
}

int sbushExit(char **args, char *envp[]) {
	printf("******************** EXITING ********************\n");
	return -1;
}

int export(char **args, char *envp[]) {
	return 0;
}
/* 
* Parses a line and returns the number of tokens generated
*/

int count_tokens(char *line) {
	int charPos = 0, countTokens = 0;
	while(line[charPos] != '\0') {
		if (line[charPos] == ' ' || line[charPos] == '\t') countTokens++;
		charPos++;
	}
	return countTokens+1;
}

int parseLine(char *line, char **args, int *pipeCount) {

  int charPos = 0, tokenIndex = 0, charTokenPos = 0;
  while((line[charPos] != '\n') && (line[charPos] != '\0')) {
        charTokenPos = 0;
        while(line[charPos] != '\n' && (line[charPos] != ' ' && line[charPos] != '\t') && line[charPos] != '\0') {
          args[tokenIndex][charTokenPos] = line[charPos];
          if(line[charPos] == '|') *pipeCount = *pipeCount + 1;
          charPos++;charTokenPos++;
        }

        if(line[charPos] == ' ' || line[charPos] == '\t') {
                charPos++;
                while(line[charPos] == ' ') charPos++;
        }
        args[tokenIndex][charTokenPos] = '\0';
	//printf("tokenIndex %d token %s\n",tokenIndex,args[tokenIndex]);
        tokenIndex++;
  }
  return tokenIndex;
}

int executeCommand(char **args, int tokenCount, int pipeCount, char *envp[]) {
	for (int i = 0; i < 3; i++) {
		if (strcmp(args[0], builtInCommands[i]) == 0) {
			return (*builtInFunc[i])(args, envp);
		}
  	}
	int res = fork();
	if (res == 0) {
		//printf("args 0 %s args 1 %s\n",args[0],args[1]);
		execve(args[0],args+1,envp);
	}
	else {
		int c;
		wait(&c);
	}
	//printf("returning from exec cmd\n");
	return 0;
}

int loopTerminal(char *envp[]) {
	int exit_flag = 0;
	while(exit_flag == 0) {
		puts("sbush>");
		char *cmdline = (char *)malloc(1000);
		read(0,cmdline,1000); // what do i do with chars_read?
		//printf("sbush> String entered was %s", cmdline);
		int num_tokens = count_tokens(cmdline);
		//printf("sbush> Num tokens %d\n",num_tokens);
		char **args = (char **)malloc(num_tokens * sizeof(char *));		
		for (int i =0;i<num_tokens;i++) {
			args[i] = (char *)malloc(TOKENSIZE);
		}	
	
		int pipeCount = 0;
		#if 0
		parseLine(cmdline, args, &pipeCount);
		#endif
		int tokensParsed = parseLine(cmdline, args, &pipeCount);
		if(args[0][0] == '\0') continue;

		exit_flag = executeCommand(args, tokensParsed, pipeCount, envp);
		#if 0
		int res = fork();
        	if (res == 0) {
                	execve(args[0],args+1,envp);
        	}
        	else {
                	int c;
                	wait(&c);
        	}		
		#endif

		#if 1
		// freeing stuff starts here	
		for (int i =0;i<num_tokens;i++) {
			if (args[i] != NULL)
				free(args[i]);
		}
		free(args);
		free(cmdline);
		#endif
	}
	return 0;
}
int executeScript(char *fileName, char *envp[]) {
	// not doing this now
	printf("in exec script function\n");
	return 0;
}

int main(int argc, char *argv[], char *envp[]) {
/*	uint64_t syscallno = 9;
    	uint64_t result = 1;
    	__asm__ __volatile__("int $0x80\n\t"
                             :"=a" (result)
                             : "0"(syscallno));
*/
	if(argc > 1) {
    		executeScript(argv[1], envp);
  	}
  	else {
    		loopTerminal(envp);
  	}
	return 0;
}
