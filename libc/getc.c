#include <stdio.h>
#include <unistd.h>

int fillBuffer(FILE *fp);
int getc(FILE *fp) {
  if(fp->cnt == 0) {
      if(fillBuffer(fp) == EOF) return EOF;
    }
  else
    fp->cnt--;
  return (unsigned char) *(fp)->ptr++;
}

int fillBuffer(FILE *fp) {
  fp->cnt = read(fp->fd, fp->base, 2048);
  fp->base[fp->cnt] = EOF;
  if(fp->cnt == 0) return EOF;
  return 1;
}

