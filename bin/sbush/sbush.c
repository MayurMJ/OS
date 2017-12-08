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
	printf("inside parseline tokenIndex %d token %s\n",tokenIndex,args[tokenIndex]);
        tokenIndex++;
  }
  return tokenIndex;
}
int executeCommand(char **args, int tokenCount, int pipeCount, char *envp[]) {
	printf("inside exec command: tokens parsed %d\n",tokenCount);
        for(int i =0;args[i] != NULL;i++) {
        	printf("%d:%s ",i,args[i]);
        }
        printf("\n");

#if 0
	for (int i = 0; i < 3; i++) {
		if (strcmp(args[0], builtInCommands[i]) == 0) {
			return (*builtInFunc[i])(args, envp);
		}
  	}
	int res = fork();
	if (res == 0) {
		//printf("args 0 %s args 1 %s\n",args[0],args[1]);
		if (tokenCount == 1)
			execve(args[0],NULL,envp);
		else
			execve(args[0],args+1,envp);
	}
	else {
		int c;
		wait(&c);
	}
	//printf("returning from exec cmd\n");
#endif
	return 0;
}

int Scan_envp(char *key, char **envp) {
	int i =0;
	if (envp == NULL) return -1;
	for (i=0; envp[i] != NULL; i++) {
		char *tempstr = envp[i];
		char *tempkey = key;
		int equal = 1;
		while ((*tempstr != '=') && (*tempkey != '\0')) {
			if (*tempstr != *tempkey) {
				equal = 0; break;
			}
			tempstr++; tempkey++;
		}
		if ((equal == 1) && (*tempstr == '=') && (*tempkey == '\0')) { // equal flag hasnt been turned to 0 by inequality test
			return i; //the index at which we will find 
		} 
	}
	return -1;
}

char **appendEnvp(char *key, char *tempstr, char **envp) {
	// if key is already there in envp then replace it with this new one
	//else create bigger envp, copy old ones and add new one
	int index = Scan_envp(key,envp);
	if (index != -1) { // add to existing entry
		envp[index] = tempstr;
		return envp;
	}
	// add new entry, this is gonna be painful :(
	int count = 0;
	if (envp == NULL) {
		char **duplenvp = (char **)malloc(2*sizeof(char *));
		duplenvp[0] = tempstr;
		duplenvp[1] = NULL;
		return duplenvp;
	}

        while (envp[count] != NULL) count++;
	
	char **duplenvp = (char **)malloc((count+2) *sizeof(char *)); // 2 extra-1 for null another for new one
	for (int i = 0;i <count;i++) {
		duplenvp[i] = envp[i];
	}
	duplenvp[count] = tempstr;
	duplenvp[count+1] = NULL;
	free(envp);
	return duplenvp;	
}

char **Set_env(char **args, char **envp) {
	char *key; char *value;
	int key_len=0, value_len=0;

	if (args[1] == NULL) {
		printf("Missing arguments to setenv\n");
		return envp;
	}
	key_len = strlen(args[1]); // strlen return 1 more than the length of str, includes \0 char
	key = args[1];

	if (args[2] == NULL) {
		printf("Missing value for key\n"); //cant set anything to null then,is that okay?
		return envp;
	}
	value_len = strlen(args[2]);
	value = args[2];

	char *finalstr = (char *)malloc(key_len + value_len); //key=value\0
	char *tempstr = finalstr;
	// copy key first to finalstr
	while(*key) {
		*tempstr = *key;
		key++; tempstr++;
	}
	*tempstr = '=';
	tempstr++;
	while(*value) {
                *tempstr = *value;
                value++; tempstr++;
        }
	*tempstr = '\0';
	return appendEnvp(args[1],finalstr,envp);
}

int loopTerminal(char *envp[]) {
	int exit_flag = 0;
	while(exit_flag == 0) {
		puts("sbush>");

		char *cmdline = (char *)malloc(1000);
		read(0,cmdline,1000); // what do i do with chars_read?

		printf("sbush> String entered was %s", cmdline);

		int num_tokens = count_tokens(cmdline);

		printf("sbush> Num tokens %d\n",num_tokens);

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
		
		printf("after parseline: tokens parsed %d\n",tokensParsed);
		for(int i =0;args[i] != NULL;i++) {
			printf("%d:%s ",i,args[i]);
		}
		printf("\n");
		
		// code for getenv and setenv
		if (strcmp(args[0], "setenv") == 0) {
			envp = Set_env(args, envp);
			int count = 0;
			while(envp[count] != NULL) {
				printf("%d %s\n",count,envp[count]);
				count++;
			}
		}
		else if (strcmp(args[0], "getenv") == 0) {
			//Get_env(args, envp);
		}
		
		/*
		exit_flag = executeCommand(args, tokensParsed, pipeCount, envp);
		*/
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
