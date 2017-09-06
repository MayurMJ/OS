#include <string.h>
#include <unistd.h>
#include <stdio.h>
int execvpe(const char *file, char *const argv[], char *const envp[]) {
  execve(file, argv, envp);
  int i = 0, j = 0, flag = 0, k = 0, l = 0;
  char envVar[256];
  char values[50][1024];
  while(envp[i][0] != '\0') {
    j = 0;
    while(envp[i][j] != '='  && envp[i][j] != '\0') {
      envVar[j] = envp[i][j];
      j++;
    }
    envVar[j] = '\0';
    j++;
    if(strcmp(envVar, "PATH") == 0) {
      flag = 1;
      break;
    }
   i++;
  }
  if(flag == 1) {
    l = 0;
    while(envp[i][j] != '\0') {
      while(envp[i][j] != ':' && envp[i][j] != '\0') {
          values[k][l] = envp[i][j];
          j++;l++;
      }
      values[k][l] = '/'; l++;
      values[k][l] = '\0';
      if(envp[i][j] != '\0')
       { k++;j++;l=0; }
      else
        k++;l=0;
    }
  }
  for(i = 0; i < k; i++) {
    strcat(values[i], file);
    execve(values[i], argv, envp);
    i++;
  }
  return -1;
}
