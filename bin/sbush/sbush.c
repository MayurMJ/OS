#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
    puts("sbush> ");
    char s[10];
     s[0] = getchar();
     s[1] = '\0';
    //char *s = "sbush";
    //int i = 0;
    //for(i = 0; i < 1000000;) {
     //i = sizeof(s);
    //}
    puts(s);
    return 0;
}
