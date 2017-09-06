#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
  //putchar(argc);
  puts(argv[1]);
  puts("sbush> ");
  FILE *fp = fopen("file.txt", "r");
  char ch;
  while((ch = getc(fp)) != EOF)
  putchar(ch);
  chdir("..");
  char wd[100];
  getcwd(wd, 100);
  puts(wd);
  return 0;
}
