#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#define BUFFERSIZE 10000
#define MAXNUMTOKENS 100
#define TOKENSIZE 100

int loopTerminal();
int readLine(char*);
int parseLine(char*, char [][TOKENSIZE], int*);
int executeScript(char *);
int executeCommand(char [][TOKENSIZE], int, int);

int main(int argc, char *argv[], char *envp[]) {
  if(argc > 1) {
    executeScript(argv[1]);
  }
  else {
    loopTerminal();
  }
  return 0;
}

int loopTerminal() {
  int buffSize = BUFFERSIZE, i = 0, tokensParsed = 0, status = 0, pipeCount = 0;
  int numTokens = MAXNUMTOKENS;
  char line[buffSize];
  char args[numTokens][TOKENSIZE];
  while(status == 0) {
    puts("sbush> ");
    readLine(line);
    tokensParsed = parseLine(line, args, &pipeCount);
    status = executeCommand(args, tokensParsed, pipeCount);
  }
  for(i = 0; i < tokensParsed; i++) {
    printf("%s\n", args[i]);
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
int executeScript(char *fileName) {
  int buffSize = BUFFERSIZE, i = 0, tokensParsed = 0, pos = 0, commentFlag = 0, parseCount = 0;
  int numTokens = MAXNUMTOKENS;
  char line[buffSize];
  char args[numTokens][TOKENSIZE];
  char *checkStart = "#!sbush", ch;
  FILE *fp = fopen(fileName,"r");
  puts("sbush> ");
  if(!fp) return 0;
  /* Check if script starts with #!sbush*/
  for(i = 0; i < 7; i++) {
    ch = fgetc(fp);
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
    else {
      line[pos] = '\0';
      tokensParsed = parseLine(line, args, &parseCount);
      pos = 0;
      commentFlag = 0;
      for(i = 0; i < tokensParsed; i++) {
        printf("%s ", args[i]);
      }
    }
  }
  line[pos] = '\0';
  tokensParsed = parseLine(line, args, &parseCount);
  for(i = 0; i < tokensParsed; i++) {
    printf("%s ", args[i]);
  }
  return 0;
}
int executeCommand(char args[][TOKENSIZE], int tokenCount, int pipeCount) {
  pid_t pid;
  int i = 0, j = 0, k = 0;
  typedef char *commands[tokenCount];
  commands c[pipeCount+1];
  for(i = 0; i < tokenCount; i++) {
    j = 0;
    while(args[i][0] != '|' && i < tokenCount) {
      c[k][j] = args[i];
      j++;i++; 
    }
    c[k][j] = (char*)0; 
    k++;
  }
  //char *argv[] =  {"ls", (char*)0};
  for(i = 0; i < pipeCount + 1; i++) {
    pid = fork();
    if(pid < 0) {
      return 1;
    }
    else if (pid == 0) {
      if (execvp(c[i][0], c[i]) == -1) {
        perror("sbush");
        return -1;
      }
    }
    else {
      //wait(NULL);
    }
  }
  return 0;
}
