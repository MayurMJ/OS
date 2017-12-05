#include <unistd.h>
#include <stdlib.h>
#include <sys/defs.h>
#include <stdio.h>
#include <dirent.h>
#include <signal.h>

int getsysid() {
	int a = 3;
	int b = 8;
	b = b*a*a;
	return b;
}
//int result;
int main(int argc, char *argv[], char *envp[]) {
 	char fileName[100];
	getcwd(fileName, 100);
	DIR *fd = opendir(fileName);
	ldirent *ld = readdir(fd);
	while(ld != NULL) {
		puts(ld->d_name);
		free(ld);
		ld = readdir(fd);
	}

	//puts ("Started");
/*	puts ("Started");
	char *temp1 = (char*) malloc(4132);
	char *temp2 = (char*) malloc(8124);
	char *temp3 = (char*) malloc(124);
	free(temp2);
	char *temp4 = (char*) malloc(100);
	int i ;
	for(i =0; i < 4; i++) {
		temp1[i] = 'm';
		temp3[i] = 'a';
		temp4[i] = 'y';
	}
	temp1[i] = '\0';
	temp3[i] = '\0';
	temp4[i] = '\0';

	puts (temp1);
	puts (temp3);
	puts (temp4);*/
	

	//char *temp = (char *)malloc(20);
	//free(temp);
	/*
	// open read write close
//	int x = open("/rootfs/bin/sample", 3);
	//                __asm__ __volatile__("int $0x80\n\t"
        //                             :"=a" (n)
        //                             : "0"(x + 100));
	*/
	/*	
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
	getcwd(buf, 100);*/
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
	
/*	int res = fork();
	if(res == 0) {
		
		char *binary = "bin/cat";
		char *s1 = "/rootfs/bin/sample";
        	char *argv[2];
        	argv[0] = s1;
        	argv[1] = '\0';
  		execve(binary, argv, NULL);
		
		//execve(binary, NULL, NULL);		
	}
	else {
		int status;
		wait(&status);
	}
*/	
	/*int res = fork();
        if(res == 0) {
<<<<<<< 1b62bfbd3103890b0bcf50c03a55e47f3b98175b
                
                char *binary = "bin/sbush";
                execve(binary, NULL, NULL);
=======
        	puts("child\n");        
//                char *binary = "bin/sbush";
  //              execve(binary, NULL, NULL);
>>>>>>> add kill
                
                //execve(binary, NULL, NULL);           
        }
        else {
<<<<<<< 1b62bfbd3103890b0bcf50c03a55e47f3b98175b
                int status;
                wait(&status);
        }*/
	//	puts("parent\n");
          //      int status;
        //        wait(&status);
//	fork();
/*	//puts("testing puts from init");
*/	//while(1); //no need to return from bin/init
	
//	while(1); //no need to return from bin/init
	return 0;
}
