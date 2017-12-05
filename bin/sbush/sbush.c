#include <unistd.h>
#include <stdlib.h>
#include <sys/defs.h>
#include <stdio.h>

#define TOKENSIZE 100

/* 
* Parses a line and returns the number of tokens generated
*/
/*
int count_tokens(char *line) {
	int charPos = 0, countTokens = 0;
	while(line[charPos] != '\0') {
		if (line[charPos] == ' ' || line[charPos] == '\t') countTokens++;
		charPos++;
	}
	return countTokens+1;
}

int parseLine(char *line, char args[][TOKENSIZE], int *pipeCount) {

  int charPos = 0, tokenIndex = 0, charTokenPos = 0;
  while(line[charPos] != '\0') {
        charTokenPos = 0;
        while((line[charPos] != ' ' && line[charPos] != '\t') && line[charPos] != '\0') {
          args[tokenIndex][charTokenPos] = line[charPos];
          if(line[charPos] == '|') *pipeCount = *pipeCount + 1;
          charPos++;charTokenPos++;
        }
        if(line[charPos] == ' ' || line[charPos] == '\t') {
                charPos++;
                while(line[charPos] == ' ') charPos++;
        }
        args[tokenIndex][charTokenPos] = '\0';
        tokenIndex++;
  }
  return tokenIndex;
}


int loopTerminal(char *envp[]) {
	int pipeCount, tokensParsed;
	
	//int buffSize = BUFFERSIZE, i = 0, tokensParsed = 0, status = 0, pipeCount = 0;
	//int numTokens = MAXNUMTOKENS;
	//char line[buffSize];
	//char args[numTokens][TOKENSIZE];
	//initCharArr(args,numTokens);
	while(status == 0) {
		puts("sbush>");
		
		char *cmdline = (char *)malloc(1000);
		int chars_read = read(0,cmdline,1000); // what do i do with chars_read?

		pipeCount = 0;
		// first parse lines to see how many tokens there are, then allocate a 2d array of that size
		int num_tokens = count_tokens(cmdline);
		char **args = (char **)kmalloc(num_tokens*TOKENSIZE);
		tokensParsed = parseLine(cmdline, args, &pipeCount);

		if(args[0][0] == '\0') continue;
		puts("sbush>");
		
		//status = executeCommand(args, tokensParsed, pipeCount, envp);
		
		free(args);
		free(cmdline);
	}
	return 0;
}
int executeScript(char *fileName, char *envp[]) {
	// not doing this now
	return 0;
}
*/
int main(int argc, char *argv[], char *envp[]) {
	int x=4096;
	char c= 'G';
	char *str = "test str";
	printf("hi from sbush %x %d %c %s\n",x,x,c,str);
/*	uint64_t syscallno = 9;
    	uint64_t result = 1;
    	__asm__ __volatile__("int $0x80\n\t"
                             :"=a" (result)
                             : "0"(syscallno));
*/
/*	if(argc > 1) {
    		executeScript(argv[1], envp);
  	}
  	else {
    		loopTerminal(envp);
  	}
*/	return 0;
}
