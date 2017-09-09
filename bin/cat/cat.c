#include <stdio.h>


int main(int argc, char *argv[], char *envp[]) {
  if(argc == 1) {
    puts("please Enter a fileName"); 
    return 0;
  }
  char *fileName = argv[1];
  FILE *fp = fopen(fileName, "r");
  char ch;
  while ((ch = getc(fp)) != EOF) {
    putchar(ch);
  }
  return 0;
}
