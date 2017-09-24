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
int memset(void *bufptr, int val, int size) {
  int i = 0;
  while(i < size) {
    *((int *)bufptr) = 0;
    bufptr++;
    i++;
  }
 return i;
}
