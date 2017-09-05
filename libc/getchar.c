#include <stdio.h>
#include <unistd.h>
int getchar()
{
  // write character to stdout
  int c;
  read(0, &c, 1);
  return c;
}

