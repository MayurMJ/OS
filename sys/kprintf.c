#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/stdarg.h>
#include <sys/kmemcpy.h>

static int colIndex = 0;
static int rowIndex = 0;

#define X_AXIS	80
#define Y_AXIS	200
#define BUFOVFLOW	2
#define WRNGFMT	1
char buffer[Y_AXIS][X_AXIS];
int buffer_row = 0;
int buffer_col = 0;

void display() {
  int i = 0, j = colIndex/2, k = 0;
  char *vidMem = (char*)0xb8000;
  char *tempMem = vidMem;
  for(k = 0; k < rowIndex; k++) {
    tempMem += 160;
  }
  while(1) {
    if(rowIndex < 24) {
      while(buffer[i][j] != '\0' && colIndex < 160) {
        tempMem[colIndex] = buffer[i][j];
        colIndex += 2; j++;
      }
      if(buffer[i][j] == '\0') break;
      rowIndex++; i++; colIndex = 0; j = 0;
      tempMem += 160;
    }
    else {
      tempMem = vidMem;
      for(k = 0; k < 23; k++) {
        memcpy(tempMem, tempMem + 160, 160);
        tempMem += 160;
      }
      tempMem = tempMem - 160;
      while(buffer[i][j] != '\0' && colIndex < 160) {
        tempMem[colIndex] = buffer[i][j];
        colIndex += 2; j++;
      }
      if(buffer[i][j] == '\0') break;
      rowIndex++; i++; colIndex = 0; j = 0;
      tempMem += 160;
    }
  }
}



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
		buffer[buffer_row][buffer_col] = c;
		buffer_col = (buffer_col + 1)%80;
		if (buffer_col == 0) buffer_row++;
		break;
	}
	return 0;
		
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
void init_buffer() {
	int i,j;
	for(i=0;i<200;i++)
		for(j=0;j<80;j++)
			buffer[i][j]= ' ';
	return;
}
void kprintf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	const char *tempfmt=fmt;
	int error=0;
	init_buffer();
        buffer_col = colIndex / 2;
	while (*tempfmt != '\0') {
		if (*tempfmt == '%' && *(tempfmt+1)) {
			char *str = NULL;
			switch(*(tempfmt+1)) {
			case 's':
				str = va_arg(args, char *);
				error = put_str_into_buffer(str);
				break;
			case 'c':;
				char charval;
				charval = va_arg(args, int);
				error = put_char_into_buffer(charval);
				break;
			case 'x':;
				int hexval;
				hexval = va_arg(args, int);
				char strhex[17];
                                str = process_hex(hexval, strhex);
				error = put_str_into_buffer(str);
				break;
			case 'p':;
				uint64_t ptrval;
				ptrval = (uint64_t)va_arg(args, void *);
				char strptr[19];
                                str = process_ptr(ptrval, strptr);
				error = put_str_into_buffer(str);
				break;
			case 'd':;
				int intval;
				intval = va_arg(args, int);
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
//			printf("%c\n",*tempfmt);
			error = put_char_into_buffer(*tempfmt);
			if (error)
                                break;
			tempfmt += 1;
		}
		
	}
	if (buffer_row != Y_AXIS) {
//		printf("check null %d %d\n",buffer_row,buffer_col);
		buffer[buffer_row][buffer_col] = '\0';
	}
	//handle error here, wrong format and buffer overflow	
	va_end(args);	
	//int i,j;
//	printf("elems %c%c%c%c\n",buffer[0][0],buffer[0][1],buffer[0][2],buffer[0][3]);
	/*for(int i = 0;i<= buffer_row;i++) {
		for(j=0;j<80;j++) {
			printf("%c",buffer[i][j]);
		}
		printf("\n");
	}*/
	display();
}
