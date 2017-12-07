#define MAXLEN 100
//static char* iterator;
#include <sys/defs.h>
int atoi(char *str)
{
    int res = 0; // Initialize result
  
    // Iterate through all characters of input string and
    // update result
    for (int i = 0; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';
  
    // return result.
    return res;
}
int strlen(char *s) {
	int len = 0;
	while(s[len] != '\0') {
		len++;
	}
	return (len+1);
}

void strcpy(char *s1, char*s2) {
   while((*s1++ = *s2++));
}

int strcmp(char *s1, char *s2) {
  while(*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strcat(char *s1, const char *s2) {
  while(*s1)
    s1++;
  while((*s1++ = *s2++));
  return 0;
}

/*char *strtok(char *s, char tok) {
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
}*/

char *strrem(char *s, char tok) {
	int len = strlen(s);
	len-=3;
	while(s[len] != tok && len >= 0) {
		s[len] = '\0';
		len--;
	}
	return s;
}

int memcpy(char *s1, char *s2, int size) {
  int i = 0;
  while(i < size) {
    *s1 = *s2;
    s1 ++;
    s2 ++;
    i++;
  }
  return i;
}

int memset(uint8_t *bufptr, int val, int size) {
  int i = 0;
  while(i < size) {
    *(bufptr) = val;
    bufptr++;
    i++;
  }
 return i;
}
