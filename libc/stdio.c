#include <stdio.h>
#include <unistd.h>


int putchar(int c)
{
  write(1, &c, 1);
  return c;
}

int puts(const char *s)
{
  // NOTE: Not entirely sure if this will work
  for( ; *s; ++s) if (putchar(*s) != *s) return EOF;
  return EOF;
}

int getc(int fd) {
  int buf[2]; // use kmalloc?
  int len=read(fd, buf, 1);
  if (len == -1) return EOF;
  return buf[0];
}
