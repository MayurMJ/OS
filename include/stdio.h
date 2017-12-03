#ifndef _STDIO_H
#define _STDIO_H

static const int EOF = -1;
static const int BUFSIZE = 2048;
static const int OPENMAX = 10;

//typedef struct _iobuf {
//  int cnt;        /* Characters left */
//  char *ptr;      /* next character position */
//  char base[2048];     /* location of buffer */
//  int fd;         /* file descriptor */
//  int flag;
//} FILE;

int putchar(int c);
int puts(const char *s);
int printf(const char *format, ...);
int getchar();
//FILE *fopen(const char *, const char *);
//int getc(FILE *);
//FILE *fopen(const char *, const char *);
//int getc(FILE *);
int getc(int fd); //changed getc to take fd instead of File *
char *gets(char *s);
#endif
