#ifndef _STRING_H
#define _STRING_H

#include <sys/defs.h>

int atoi(char *str);
void strcpy(char *s1, char*s2);
char *strrem(char *s, char tok);
int strlen(char *s);
int strcmp(char *s1, char *s2);
int strcat(char *s1, const char *s2);
char* strtok(char *s1, char s2);
int memcpy(char *s1, char *s2, int size);
int memset(uint8_t *bufptr, int val, int size);

#endif
