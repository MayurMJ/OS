#include <string.h>
#include <stdio.h>
void setenv(char *name, char *value, char *envp[]) {
  int i = 0, j = 0, k = 0, l = 0;
  char envVar[256];
  while(envp[i]!= 0) {
    j = 0;
    while(envp[i][j] != '='  && envp[i][j] != '\0') {
      envVar[j] = envp[i][j];
      j++;
    }
    envVar[j] = '\0';
    if(strcmp(name, envVar) == 0) {
      while(name[l] != '\0') {
        envp[i][k] = name[l];
        k++;l++;
      }
      envp[i][k] = '=';
      k++;
      l = 0;
      while(value[l] != '\0') {
	envp[i][k] = value[l];
	k++;l++;
      }
      envp[i][k] = '\0';
      break; 
    }
    i++;
  }
  char addVar[256];
  while(name[l] != '\0') {
     addVar[k] = name[l];
     k++;l++;
  }
  addVar[k] = '=';
  k++;
  l = 0;
  while(value[l] != '\0') {
    addVar[k] = value[l];
    k++;l++;
  }
  addVar[k] = '\0';
  envp[i] = addVar;
  envp[i+1] = (char*)0;
}
