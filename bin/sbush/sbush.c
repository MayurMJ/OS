#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define BUFFERSIZE 10000
#define MAXNUMTOKENS 100
#define TOKENSIZE 100


char sbush[100] = "sbush>";

int loopTerminal(char *envp[]);
int readLine(char*);
int parseLine(char*, char [][TOKENSIZE], int*);
int executeScript(char *, char *envp[]);
int executeCommand(char [][TOKENSIZE], int, int, char *envp[]);
int cd(char[][TOKENSIZE], char *envp[]);
int export(char[][TOKENSIZE], char *envp[]);
int sbushExit(char[][TOKENSIZE], char *envp[]);
int stringCmp(char *, char *);
int initArr(int *, int size);
int initCharArr(char [][TOKENSIZE], int size);

char *builtInCommands[] = {
  "cd",
  "export",
  "exit"
};

int (*builtInFunc[]) (char [][TOKENSIZE], char *envp[]) = {
  &cd,
  &export,
  &sbushExit
};

int main(int argc, char *argv[], char *envp[]) {
  if(argc > 1) {
    executeScript(argv[1], envp);
  }
  else {
    loopTerminal(envp);
  }
  return 0;
}

int loopTerminal(char *envp[]) {
  int buffSize = BUFFERSIZE, i = 0, tokensParsed = 0, status = 0, pipeCount = 0;
  int numTokens = MAXNUMTOKENS;
  char line[buffSize];
  char args[numTokens][TOKENSIZE];
  initCharArr(args,numTokens);
  while(status == 0) {
    puts(sbush);
    readLine(line);
    pipeCount = 0;
    initCharArr(args,numTokens);
    tokensParsed = parseLine(line, args, &pipeCount);
    if(args[0][0] == '\0') continue;
    status = executeCommand(args, tokensParsed, pipeCount, envp);
  }
  for(i = 0; i < tokensParsed; i++) {
    //printf("%s\n", args[i]);
  }
  return 0;
}
/*
* Reads a line from console
*/
int readLine(char *line) {
  unsigned char c;
  int pos = 0;
  while(1) {
    c = getchar();
    if(c == '\n') {
      line[pos] = '\0';
      break;
    }
    else {
      line[pos] = c;
    }
    pos++;
  }
  return 0;
}
/* 
* Parses a line and returns the number of tokens generated
*/
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
/*
* Executes the script whose name is provided in filename
*/
int executeScript(char *fileName, char *envp[]) {
  int buffSize = BUFFERSIZE, i = 0, tokensParsed = 0, pos = 0, commentFlag = 0, parseCount = 0, pipeCount = 0, status = 0;
  int numTokens = MAXNUMTOKENS;
  char line[buffSize];
  char args[numTokens][TOKENSIZE];
  char *checkStart = "#!/rootfs/bin/sbush", ch;
  FILE *fp = fopen(fileName,"r");
  if(!fp) return 0;
  /* Check if script starts with #!sbush*/
  for(i = 0; i < 18; i++) {
    ch = getc(fp);
    if(ch != checkStart[i]) return 0;
  }
  /*Skip the first Line*/
  while((ch = getc(fp)) != '\n');
  while ((ch = getc(fp)) != EOF) {
    if(ch != '\n') {  
      if (ch == '#')
      {
        line[pos] = '\0';
        commentFlag = 1;
      }
      else {
        if(commentFlag == 1) {
          line[pos] = '\0';
        }
        else {
          line[pos] = ch;
          pos++;
        }
      }
    }
    else if(pos != 0) {
      pipeCount = 0;
      //puts("sbush>");
      initCharArr(args,numTokens);
      line[pos] = '\0';
      line[pos+1] = '\n';
      //puts(line); 
      tokensParsed = parseLine(line, args, &parseCount);
      pos = 0;
      commentFlag = 0;
      status = executeCommand(args, tokensParsed, pipeCount, envp);
      if(status == -1) break;
      //for(i = 0; i < tokensParsed; i++) {
        //printf("%s ", args[i]);
      //}
    }
  }
  pipeCount = 0;
  //puts("sbush>");
  initCharArr(args,numTokens);
  line[pos] = '\0';
  tokensParsed = parseLine(line, args, &parseCount);
  pos = 0;
  commentFlag = 0;
  status = executeCommand(args, tokensParsed, pipeCount, envp);

  return 0;
}
int executeCommand(char args[][TOKENSIZE], int tokenCount, int pipeCount, char *envp[]) {
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
  //printf("fdin = %d  fdout = %d\n",fdin,fdout);
  //char *argv[] =  {"ls", (char*)0};
  for(i = 0; i < pipeCount + 1; i++) {
    dup2(fdin,0);
    close(fdin);
    if(i==pipeCount) {//last command
      fdout = dup(mystdout);
      //close(mystdout);
    }
    else {
       int fd[2];
       if(pipe(fd)!=0){ puts("ERROR PIPE"); }
       fdin = fd[0];
       fdout = fd[1];
    }
    
    dup2(fdout,1);
    close(fdout);
   /* if(c[i][0][0] == '\0') {
  dup2(mystdin,0);
  dup2(mystdout,1);
  close(mystdin);
  close(mystdout);
   return 0;
    }*/
    pid = fork();
    if(pid < 0) {
      return 1;
    }
    else if (pid == 0) {
      if (execvpe(c[i][0], c[i], envp) == -1) {
        puts("No such command  \n");
        return -1;
      }
    }
//    else if(backgroundEnable[i] == 0) {
//      waitpid(pid, NULL);
//    }
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
int cd(char args[][TOKENSIZE], char *envp[])
{
  if (args[1] != NULL) {
    if (chdir(args[1]) != 0) {
      //perror("sbush");
    }
  }
  return 0;
}
int export(char args[][TOKENSIZE], char *envp[]) {
  char nameValue[2][100];
  int i =0, j = 0, m = 0;
  if(args[1] != NULL) {
    while(args[1][i] != '=') {
      nameValue[0][i] = args[1][i];
      i++;
    }
    nameValue[0][i] = '\0';
    i++;
    while(args[1][i] != '\0') {
      nameValue[1][j] = args[1][i];
      i++; j++;
    }
    nameValue[1][j] = '\0';
    if(strcmp(nameValue[0], "PS1") == 0) {
      while(nameValue[1][m] != '\0') {
        sbush[m] = nameValue[1][m];
        m++;
      }
     sbush[m] = '>';
     sbush[m+1] = '\0';
    }
    setenv(nameValue[0], nameValue[1], envp);
  }
  return 0;
}
int sbushExit(char args[][TOKENSIZE], char *envp[]) {
  return -1;
}
int stringCmp(char *s1, char *s2) {
  while(*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
int initArr(int *arr, int size) {
  int i = 0;
  for(i = 0; i < size; i++) {
    arr[i] = 0;
  }
  return 0;
}
int initCharArr(char arr[][TOKENSIZE], int size) {
  int i = 0;
  for(i = 0; i < size; i++) {
    arr[i][0] = '\0';
  }
  return 0;
}
