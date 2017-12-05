#include <unistd.h>
#include <stdlib.h>
#include <sys/defs.h>
#include <stdio.h>
#include <string.h>

#define TOKENSIZE 100

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
	printf("tokenIndex %d token %s\n",tokenIndex,args[tokenIndex]);
        tokenIndex++;
  }
  return tokenIndex;
}


int loopTerminal(char *envp[]) {
	int exit_flag = 0;
	while(exit_flag == 0) {
		puts("sbush>");
		
		char *cmdline = (char *)malloc(1000);
		read(0,cmdline,1000); // what do i do with chars_read?

		if (strcmp(cmdline, "exit\n") == 0) {
			exit_flag = 1;
			continue;
		}
		printf("sbush> String entered was %s", cmdline);
		
		// first parse lines to see how many tokens there are, then allocate a 2d array of that size
		int num_tokens = count_tokens(cmdline);
		printf("sbush> Num tokens %d\n",num_tokens);


		char **args = (char **)malloc(num_tokens * sizeof(char *));		
		for (int i =0;i<num_tokens;i++) {
			args[i] = (char *)malloc(TOKENSIZE);
		}	
	
		int pipeCount = 0;
		int tokensParsed = parseLine(cmdline, args, &pipeCount);
		printf("sbush> num tokens from parseline %d\n",tokensParsed);	
		if(args[0][0] == '\0') continue;
			
		//status = executeCommand(args, tokensParsed, pipeCount, envp);
		
		// freeing stuff starts here	
		for (int i =0;i<num_tokens;i++) {
			free(args[i]);
		}
		free(args);
		free(cmdline);
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
