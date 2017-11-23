#include <sys/defs.h>
int memcpy(char *s1, char *s2, int size) {
  int i = 0;
  while(i < size/2) {
    *s1 = *s2;
    s1 += 2;
    s2 += 2;
    i++;
  }
  return i;
}
int kmemcpy(char *s1, char *s2, int size) {
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
