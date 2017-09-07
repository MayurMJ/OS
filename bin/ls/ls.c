#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <syscall.h>

int main()
{

  char buf2[2048];
  char * p = getcwd(buf2, 2048);
  if (!p) {
    puts("can't find the process directory\n");
    return -1;
  }
 // puts(buf2);
//  puts("\n");
/*
  DIR * dirs = opendir(buf);
  if (!dirs) {
    puts("can't open directory\n");
    return -1;
  }
  ldirent * curr = readdir(dirs);
  if (!curr) {
    puts("can't read ldirent\n");
    return -1;
  }
  puts(curr->d_name);
  return 0;*/
/*  puts("My ls\n");
  return 0;*/
  int fd,nread;
  char buf[10000];
  ldirent * d;
  int cpos;
  //char d_type;
  fd = open(buf2,0);
  if(fd == -1) {
    puts("cant open directory");
  }
  nread = getdents(fd,(ldirent *)buf,10000);
  if (nread == -1)
    puts("getdents nread = -1");
  if (nread == 0)
    puts("getdents nread = -0");
  for (cpos = 0; cpos < nread;) {
    d = (ldirent *) (buf + cpos);
    puts(" ");
    puts(d->d_name);
    puts(" ");
    cpos += d->d_reclen;
  }
 puts("\n");
 return 0;
}

