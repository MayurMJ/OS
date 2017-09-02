#include <stdio.h>
#define BUFFERSIZE 10000
#define MAXNUMTOKENS 100
#define TOKENSIZE 100

int loopTerminal();
int readLine(char*);
int parseLine(char*, char [][TOKENSIZE]);

int main(int argc, char *argv[], char *envp[]) {
  puts("sbush> ");
  loopTerminal();
  return 0;
}
int loopTerminal() {
  int buffSize = BUFFERSIZE, i = 0, tokensParsed = 0;
  int numTokens = MAXNUMTOKENS;
  char line[buffSize];
  char args[numTokens][TOKENSIZE];
  readLine(line);
  tokensParsed = parseLine(line, args);
  for(i = 0; i < tokensParsed; i++) {
    printf("%s ", args[i]);
  }
  return 0;
}
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
int parseLine(char *line, char args[][TOKENSIZE]) {
  int i = 0, j = 0, k = 0;
  while(line[i] != '\0') {
        k = 0;
    	while((line[i] != ' ' || line[i] == '\t') && line[i] != '\0') {
          args[j][k] = line[i];
          i++;k++;
        }
        if(line[i] == ' ' || line[i] == '\t') {
		i++;
        }
	args[j][k] = '\0';
        j++;
  }
  return j;
}

