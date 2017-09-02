#include <stdio.h>
#define BUFFERSIZE 10000
#define MAXNUMTOKENS 100
#define TOKENSIZE 100

int loopTerminal();
int readLine(char*);
int parseLine(char*, char [][TOKENSIZE]);
int executeScript(char *);

int main(int argc, char *argv[], char *envp[]) {
  if(argc > 0) {
    executeScript(argv[1]);
  }
  loopTerminal();
  return 0;
}

int loopTerminal() {
  int buffSize = BUFFERSIZE, i = 0, tokensParsed = 0;
  int numTokens = MAXNUMTOKENS;
  char line[buffSize];
  char args[numTokens][TOKENSIZE];
  puts("sbush> ");
  readLine(line);
  tokensParsed = parseLine(line, args);
  for(i = 0; i < tokensParsed; i++) {
    printf("%s ", args[i]);
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
        while((line[charPos] != ' ' || line[charPos] == '\t') && line[charPos] != '\0') {
          args[tokenIndex][charTokenPos] = line[charPos];
          charPos++;charTokenPos++;
        }
        if(line[charPos] == ' ' || line[charPos] == '\t') {
		charPos++;
        }
	args[tokenIndex][charTokenPos] = '\0';
        tokenIndex++;
  }
  return tokenIndex;
}

int executeScript(char *fileName) {
  //char fileContent[][BUFFERSIZE];
  return 0;
}
