#ifndef _KSTRING_H
#define _KSTRING_H

void kstrcpy(char *s1, char*s2);
char *kstrrem(char *s, char tok);
int kstrlen(char *s);
int kstrcmp(char *s1, char *s2);
int kstrcat(char *s1, const char *s2);
char* kstrtok(char *s1, char s2);

#endif
