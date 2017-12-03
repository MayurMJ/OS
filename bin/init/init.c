#include <unistd.h>
#include <stdlib.h>
#include <sys/defs.h>
#include <stdio.h>
#include <dirent.h>
int getsysid() {
	int a = 3;
	int b = 8;
	b = b*a*a;
	return b;
}
//int result;
int main(int argc, char *argv[], char *envp[]) {
	int n;
	/*
	// open read write close
//	int x = open("/rootfs/bin/sample", 3);
	//                __asm__ __volatile__("int $0x80\n\t"
        //                             :"=a" (n)
        //                             : "0"(x + 100));
	*/
	
	int fd = opendir("rootfs/bin");
	DIR* dir = readdir(fd);
	while(dir != NULL) {
	__asm__ __volatile("int $0x80\n\t"
                           :"=a"(n)
                           :"a"(1),"D"(1),"S"(dir->d_name),"d"(100));
		//puts(dir->d_name);
		dir = readdir(fd);
	}
	closedir(fd);
	char buf[100];
	chdir("../../bin");
	getcwd(buf, 100);
//	puts(buf);
	//close(x);
	// read from stdin and print in a loop
        //uint64_t buffer[512];
	/* for(int x=0;x<10;x++) {
        uint64_t buffer[512];
        __asm__ __volatile("int $0x80\n\t"
                           :"=a"(retval)
                           :"a"(0),"D"(0),"S"(buffer),"d"(count));
	__asm__ __volatile("int $0x80\n\t"
			   :"=a"(retval)
			   :"a"(59), "D"((uint64_t)buffer), "S"((uint64_t)0), "d"((uint64_t)0)
			   :"memory");
	}
	char *strtemp = "gargi";
	int retval;
	__asm__ __volatile("int $0x80\n\t"
                           :"=a"(retval)
                           :"a"(1),"D"(1),"S"((uint64_t)strtemp),"d"(0));
	*/
	/*	
	// cat code
	if(argc == 1) {
    		puts("please Enter a fileName");
  	}
  	char *fileName = argv[1];
	int fd = open(fileName, 3);
  	//FILE *fp = fopen(fileName, "r");
  	char ch;
	
  	while ((ch = getc(fd)) != EOF) {
    		putchar(ch);
  	}
	*/
	int res = fork();
	if(res == 0) {
		char *binary = "bin/sbush";
  		execve(binary, NULL, NULL);		
	}
	else {
		int status;
		wait(&status);
	}
	//while(1); //no need to return from bin/init
	
	while(1); //no need to return from bin/init
	return 0;
}
