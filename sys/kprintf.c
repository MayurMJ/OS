#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/stdarg.h>
#include <sys/kmemcpy.h>
void kprintf(const char *fmt, ...)
{
  int i = 0, specifierIndex = 1;
  //char accessSpecifiers[100];
  char *vidMem = (char*)0xb8000;
  //char *s;
  int d;
  va_list arguments;
  /*while(fmt[i] != '\0') {
    if(fmt[i] == '%') {
      if(fmt[i+1] != '%'){
        accessSpecifiers[countPerc] = fmt[i+1];
        i++;
    }
    i++;
  }
  i = 0; */
  va_start ( arguments, fmt );  
  long arg1 =(long) (&fmt);
  while(fmt[i] != '\0') {
    if(fmt[i] == '%') {
      switch(fmt[i+1]) {
        case 's':
         //s = va_arg(arguments , char *);
         //while(*s) {*vidMem = *s; vidMem += 2; s += 1;}
         break;
        case 'd':
         d = va_arg(arguments, int);
         *vidMem = d;
         vidMem += 2;
         break;
         case 'c':
         *vidMem = *((char *) (arg1 + 8));
         vidMem += 2;
         break; 
      }
      i +=2;
      specifierIndex++;
    }
    else {
      *vidMem = fmt[i];
      vidMem += 2;
      i++;
    }
  }
  char *buf [120];
  char *a = "MEMcpy Testing!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
  for(i = 0; i < 100; i++) {
    buf[i] = a;
  }
  char *tempMem = vidMem;
  for(i = 0 ; i < 24; i++) {
   int j = 0, k = 0;
    while(buf[i][j] != '\0') {
      tempMem[k] = buf[i][j];
      j++; k+=2;
    }
    tempMem[k] = 49 + i;
    tempMem += 160;
  }
   tempMem = vidMem;
  for(i = 0; i < 23; i++) { 
    memcpy(tempMem, tempMem + 160, 160);
    tempMem += 160;
  }
}
