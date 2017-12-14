#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stdarg.h>

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

char *gets(char *str) {
	int len = read(0, str, 2000);
	if ((len == 0) || (len == -1)) return NULL;
	else return str;
}

char *get_ptr(uint64_t num, char *str) {
        if(num == 0) {
                str[0] = '0';
                str[1]= '\0';
                return str;
        }
        str[18] = '\0';
        int shift = 4;
        int64_t mask = 0xf;
        int str_index = 17;
        while ((str_index >= 2) && (num != 0)) {
                int val = (num & mask);
                char c;
                if (val > 9) c = val + 87;
                else    c = val + '0';
                str[str_index--] = c;
                num = (num & (~mask)) >> shift;
        }
        char *newstr = str+str_index+1;
        return newstr;
}

char *process_ptr(uint64_t num, char *str) {
        char *str2 = get_ptr(num,str);
        *(--str2) = 'x';
        *(--str2) = '0';
        return str2;
}

char *process_hex(int64_t num, char *str) {
        if(num == 0) {
                str[0] = '0';
                str[1]= '\0';
                return str;
        }
        str[8] = '\0';
        int shift = 4;
        int64_t mask = 0xf;
        int str_index = 7;
        while ((str_index >= 0) && (num != 0)) {
                int val = (num & mask);
                char c;
                if (val > 9) c = val + 87;
                else    c = val + '0';
                str[str_index--] = c;
                num = (num & (~mask)) >> shift;
        }
        char *newstr = str+str_index+1;
        return newstr;
}

char *process_int(int n, char *str) {
        int64_t num = n;
        if (num == 0) {
                str[0] = '0';
                str[1] = '\0';
                return str;
        }
        str[11] = '\0';
        int str_index = 10;
        int is_neg = 0;
        if (num < 0) {
                is_neg = 1;
                num = 0 - num;
        }
        while (num > 0) {
                char c = (num % 10) + '0';
                num = num / 10;
                str[str_index--] = c;
        }
        if (is_neg) {
                str[str_index--] = '-';
        }
        char *finstr = str + str_index + 1;
        return finstr;
}

char *add_str_to_final_str(char *src, char *dest) {
	while (*src != '\0') {
		*dest = *src;
		dest++; src++;
	}
	return dest;
}

int printf(const char *fmt, ...)
{
	char *finalStr = (char *)malloc(1000);
        va_list args;
        va_start(args, fmt);
        const char *tempfmt=fmt;
	char *tempstr = finalStr;

        while (*tempfmt != '\0') {
                if (*tempfmt == '%' && *(tempfmt+1)) {
                        char *str = NULL;
                        switch(*(tempfmt+1)) {
                        case 's':
                                str = va_arg(args, char *);
                                tempstr = add_str_to_final_str(str,tempstr);
                                break;
                        case 'c':;
                                char charval;
                                charval = va_arg(args, int);
                                *tempstr = charval;
				tempstr++;
                                break;
                        case 'x':;
                                int hexval;
                                hexval = va_arg(args, int);
                                char* strhex = (char *)malloc(9);
                                str = process_hex(hexval, strhex);
                                tempstr = add_str_to_final_str(str,tempstr);
				free(strhex);
                                break;
                        case 'p':;
                                uint64_t ptrval;
				ptrval = (uint64_t)va_arg(args, void *);
                                char *strptr = (char *)malloc(19);
                                str = process_ptr(ptrval, strptr);
                                tempstr = add_str_to_final_str(str,tempstr);
                                free(strptr);
                                break;
                        case 'd':;
                                int intval;
                                intval = va_arg(args, int);
                                char *strint = (char *)malloc(12);
                                str = process_int(intval, strint);
                                tempstr = add_str_to_final_str(str,tempstr);
                                free(strint);
                                break;
                        case '%':;
                                *tempstr = '%';
				tempstr++;
                                break;
                        default:
				puts("Wrong format!!!\n");
                                break;
                        }
                        tempfmt += 2;
                }
                else {
			*tempstr = *tempfmt;
			tempstr++;
                        tempfmt += 1;
                }
	}
	*tempstr = '\0';
	int len = (int)(tempstr-finalStr);
	int ret = write(1, finalStr, len);
        va_end(args);
	free(finalStr);
	return ret;
}
