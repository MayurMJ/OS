#ifndef _KMEMCPY_H
#define _KMEMCPY_H
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

#endif
