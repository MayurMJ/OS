#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
static int iobCount = 0;
FILE _iob[10];

FILE* fopen(const char *name, const char *mode) {
  int fd;
  FILE *fp = &_iob[iobCount];
  iobCount++;
  fd = open(name, 0);
  if(fd == -1) return NULL;
  fp->fd = fd;
  fp->cnt = 0;
  fp->ptr = &(fp->base[0]);
  return fp;
}
