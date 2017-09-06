#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
  putchar(argc);
  puts("sbush> ");
  FILE *fp = fopen("file.txt", "r");
  char ch;
  while((ch = getc(fp)) != EOF)
  putchar(ch);
  return 0;
}
