#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#define BUFFERSIZE 10000
#define MAXNUMTOKENS 100
#define TOKENSIZE 100
int initArr(char [][TOKENSIZE], int, int);
int loopTerminal();
int readLine(char*);
int parseLine(char*, char [][TOKENSIZE]);
int executeScript(char *);
int executeCommand(char [][TOKENSIZE]);

int main(int argc, char *argv[], char *envp[]) {
  if(argc > 1) {
    executeScript(argv[1]);
  }
  else {
    loopTerminal();
  }
  return 0;
}

int initArr(char args[][TOKENSIZE], int x, int y) {
  int i = 0, j = 0;
  for(i = 0; i < x; i++) {
    for(j = 0; j < y; j++) {
      args[i][j] = '\0';
    }
  }
  return 0;
}

int loopTerminal() {
  int buffSize = BUFFERSIZE, i = 0, tokensParsed = 0;
  int numTokens = MAXNUMTOKENS;
  char line[buffSize];
  char args[numTokens][TOKENSIZE];
  initArr(args, numTokens, TOKENSIZE);
  puts("sbush> ");
  readLine(line);
  tokensParsed = parseLine(line, args);
  executeCommand(args);
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
int parseLine(char *line, char args[][TOKENSIZE]) {
  int charPos = 0, tokenIndex = 0, charTokenPos = 0;
  while(line[charPos] != '\0') {
        charTokenPos = 0;
        while((line[charPos] != ' ' && line[charPos] != '\t') && line[charPos] != '\0') {
          args[tokenIndex][charTokenPos] = line[charPos];
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
  int buffSize = BUFFERSIZE, i = 0, tokensParsed = 0, pos = 0, commentFlag = 0;
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
      tokensParsed = parseLine(line, args);
      pos = 0;
      commentFlag = 0;
      for(i = 0; i < tokensParsed; i++) {
        printf("%s ", args[i]);
      }
    }
  }
  line[pos] = '\0';
  tokensParsed = parseLine(line, args);
  for(i = 0; i < tokensParsed; i++) {
    printf("%s ", args[i]);
  }
  return 0;
}
int executeCommand(char args[][TOKENSIZE]) {
  pid_t pid;
  char *argv[] = {args[0],(char*)0};
  //char *argv[] =  {"ls", (char*)0};
  pid = fork();
  if(pid < 0) {
    return 1;
  }
  else if (pid == 0) {
    if (execvp(argv[0], argv) == -1) {
      perror("sbush");
    }
  }
  else {
      //wait(NULL);
  }
  return 0;
}
