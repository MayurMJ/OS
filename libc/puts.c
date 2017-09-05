#include <stdio.h>
#include <unistd.h>

int puts(const char *s)
{
  //for( ; *s; ++s) if (putchar(*s) != *s) return EOF;
  //return (putchar('\n') == '\n') ? 0 : EOF;
  int len = 0;
  while(s[len] != '\0') {
    len++;
  }
  return write(1, s, len);  
}
