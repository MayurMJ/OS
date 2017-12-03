#define MAXLEN 100
#include <sys/kmalloc.h>
static char* iterator;
int kstrlen(char *s) {
	int len = 0;
	while(s[len] != '\0') {
		len++;
	}
	return (len+1);
}
void kstrcpy(char *s1, char*s2) {
   while((*s1++ = *s2++));
}

int kstrcmp(char *s1, char *s2) {
  while(*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
int kstrcat(char *s1, const char *s2) {
  while(*s1)
    s1++;
  while((*s1++ = *s2++));
  return 0;
}

char *kstrtok(char *s, char tok) {
  char *ret = (char *)kmalloc(sizeof(char) * MAXLEN);
  int i = 0;
  if(s != NULL) {
    iterator = s;
  }
  else {
    if(!*iterator) return NULL;
    iterator++;
    if(!*iterator) return NULL;

  }
  for(;*iterator != tok && *iterator; iterator++) {
    ret[i] = *iterator;
    i++;
  }
  return ret;
}
char *kstrrem(char *s, char tok) {
	int len = kstrlen(s);
	len-=3;
	while(s[len] != tok && len >= 0) {
		s[len] = '\0';
		len--;
	}
	return s;
}

