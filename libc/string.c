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

