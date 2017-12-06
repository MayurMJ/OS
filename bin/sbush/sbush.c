#include <unistd.h>
#include <stdlib.h>
#include <sys/defs.h>
#include <stdio.h>
#include <string.h>
#define TOKENSIZE 100
/*
int cd(char **args, char *envp[]);
int export(char **args, char *envp[]);

char *builtInCommands[] = {
	"cd",
	"export"
};

int (*builtInFunc[]) (char [][TOKENSIZE], char *envp[]) = {
	&cd,
	&export
};
*/
int cd(char **args, char *envp[])
{
	printf("in cd right now\n");
	printf("args 1 is %s\n",args[1]);
	if (args[1] != NULL) {
		if (chdir(args[1]) == -1) {
			printf("sbush> Directory not found\n");
			return -1;
    		}
  	}
	else {
		printf("sbush> Missing argument to cd");
		return -1;
	}
	printf("directory change worked\n");
  	return 0;
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
#if 0
int executeCommand(char **args, int tokenCount, int pipeCount, char *envp[]) {
	int i = 0, j = 0, k = 0;
	for (i = 0; i < 3; i++) {
		if (stringCmp(args[0], builtInCommands[i]) == 0) {
			return (*builtInFunc[i])(args, envp);
		}
	}
	pid_t pid = 0;
	typedef char *commands[tokenCount];
	int backgroundEnable[pipeCount + 1];
	initArr(backgroundEnable, pipeCount+1);
	commands c[pipeCount + 1];
	for(i = 0; i < tokenCount; i++) {
		j = 0;
		while(args[i][0] != '|' && i < tokenCount) {
			c[k][j] = args[i];
			j++;i++;
		}
		if(j-1 > 0) {
			if (c[k][j-1][0] == '&') {
				backgroundEnable[k] = 1;
				c[k][j-1] = (char*)0;
			}
		}
		c[k][j] = (char*)0;
		k++;
	}
	int mystdin = dup(0);
	int mystdout = dup(1);
	int fdin;
	fdin = dup(mystdin);
	int fdout;
	for(i = 0; i < pipeCount + 1; i++) {
		dup2(fdin,0);
		close(fdin);
		if(i==pipeCount) {//last command
			fdout = dup(mystdout);
		}
		else {
			int fd[2];
			if(pipe(fd)!=0){ puts("ERROR PIPE"); }
			fdin = fd[0];
			fdout = fd[1];
		}
		dup2(fdout,1);
		close(fdout);
		pid = fork();
		if(pid < 0) {
			return 1;
		}
		else if (pid == 0) {
			if (execvpe(c[i][0], c[i], envp) == -1) {
				return -1;
			}
		}
	}
	dup2(mystdin,0);
	dup2(mystdout,1);
	close(mystdin);
	close(mystdout);
	if(backgroundEnable[i-1] == 0) {
		waitpid(pid, NULL);
	}

	return 0;
}
#endif
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
		//printf("sbush> String entered was %s", cmdline);
		
		// first parse lines to see how many tokens there are, then allocate a 2d array of that size
		int num_tokens = count_tokens(cmdline);
		//printf("sbush> Num tokens %d\n",num_tokens);


		char **args = (char **)malloc(num_tokens * sizeof(char *));		
		for (int i =0;i<num_tokens;i++) {
			args[i] = (char *)malloc(TOKENSIZE);
		}	
	
		int pipeCount = 0;
		parseLine(cmdline, args, &pipeCount);
		//int tokensParsed = parseLine(cmdline, args, &pipeCount);
		//printf("sbush> num tokens from parseline %d\n",tokensParsed);	
		if(args[0][0] == '\0') continue;
		printf("1\n");
		if (strcmp(args[0], "cd") == 0) {
			cd(args, envp);
		}
		else if (strcmp(args[0], "ls") == 0) {
                        int res = fork();
			if (res == 0) {
				if (num_tokens > 1) { // that means ls has been given a path
					char *s1 = "bin/ls";
					char *s2 = args[1];
					//printf("2nd arg to ls is %s\n",s2);
					char *lsargv[3];
					lsargv[0] = s1; lsargv[1] = s2; lsargv[2] = '\0';
					execve("bin/ls", lsargv, NULL);
				}
				else {
					execve("bin/ls", NULL, NULL);
				}
			}
			else {
				int status;
				wait(&status);
			}
                }
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
