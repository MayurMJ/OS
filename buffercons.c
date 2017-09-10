/*
#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/stdarg.h>
*/
#define X_AXIS	80
#define Y_AXIS	200
#define BUFOVFLOW	2
#define WRNGFMT	1
char buffer[Y_AXIS][X_AXIS];
int buffer_row = 0;
int buffer_col = 0;
typedef unsigned long uint64_t;
typedef long int64_t;

char *get_ptr(uint64_t num, char *str) {
        //uint64_t num = n;
        if(num == 0) {
                str[0] = 0;
                str[1]= '\0';
                return str;
        }
        str[18] = '\0';
        int shift = 0;
        uint64_t mask = 0xf;
        int str_index = 17;
        while ((str_index > 0) && (num != 0)) {
                int val = (num & mask) >> shift;
                char c;
                if (val > 9) c = val + 87;
                else    c = val + '0';
                shift += 4;
                str[str_index--] = c;
                num = num & (~mask);
                mask = mask << 4;
        }
        char * newstr = str+str_index+1;
        return newstr;
}
//19
char *process_ptr(uint64_t num, char *str) {
	char *str2 = get_ptr(num,str);
        *(--str2) = 'x';
        *(--str2) = '0';
	return str2;	
}
//17
char *process_hex(int n, char *str) {
        int64_t num = n;
        if(num == 0) {
                str[0] = 0;
                str[1]= '\0';
                return str;
        }
        str[16] = '\0';
        int shift = 0;
        int64_t mask = 0xf;
        int str_index = 15;
        while ((str_index > 0) && (num != 0)) {
                int val = (num & mask) >> shift;
                char c;
                if (val > 9) c = val + 87;
                else    c = val + '0';
                shift += 4;
                str[str_index--] = c;
                num = num & (~mask);
                mask = mask << 4;
        }
        char * newstr = str+str_index+1;
        return newstr;
}

//12
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

int put_char_into_buffer(char c) {
	/*
	 * stuff to handle :
	 * 1) \n \t
	 * 2) updating bufferrow
	 * corner cases: \t might push it to the next line
	 */
	// to clarify, buffer_row and buffer_col needs to be updated to the 
	// next char that is to be written
	if (buffer_row >=200)
		return BUFOVFLOW;
	switch(c) {
	case '\n':
		buffer_row++;
		buffer_col = 0;
		break;
	case '\t':
		buffer_col = (buffer_col + 4) % 80;
		break;
	case '\v':
		buffer_row++;
		break;
	default:// normal chars
		buffer[buffer_row++][buffer_col] = c;
		buffer_col = (buffer_col + 1)%80;
		break;
	}
		
}
int put_str_into_buffer(char *str) {
	int err=0;
	while(*str) {
		err = put_char_into_buffer(*str);
		if (err)
			return err;
		str++;	
	}
	return 0;	
}
void kprintf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char *tempfmt=fmt;
	int error=0;
	while (*tempfmt) {
		if (*tempfmt == '%' && *(tempfmt+1)) {
			char *str = NULL;
			switch(*(tempfmt+1)) {
			case 's':
				str = va_args(args, char *);
				error = put_str_into_buffer(str);
				break;
			case 'c':
				char charval = va_args(args, int);
				error = put_char_into_buffer(charval);
				break;
			case 'x':
				int hexval = va_args(args, int);
				char strhex[17];
                                str = process_hex(hexval, strhex);
				error = put_str_into_buffer(str);
				break;
			case 'p':
				uint64_t ptrval = (uint64_t)va_args(args, void *);
				char strptr[19];
                                str = process_ptr(ptrval, strptr);
				error = put_str_into_buffer(str);
				break;
			case 'd':
				int intval = va_args(args, int);
				char strint[12];
		                str = process_int(intval, strint);
				error = put_str_into_buffer(str);
				break;
			case '%':
				error = put_char_into_buffer(*(tempfmt+1));
				break;
			default:error = WRNGFMT;
				break;
			}
			if (error != 0)
				break;
			tempfmt += 2;
		}
		else {
			error = put_char_into_buffer(*tempfmt);
			if (!error)
                                break;
			tempfmt += 1;
		}
		
	}
	if (buffer_row != Y_AXIS) {
		buffer[buffer_row][buffer_col] = '\0'
	}
	//handle error here, wrong format and buffer overflow	
	va_end(args);	
}
