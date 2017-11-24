
        int i = 0;
int main(int argc, char *argv[], char *envp[]) {
        int result; 
	int n = 57; 
        __asm__("int $0x80\n\t": "=&a" (result)
			       : "0"(n));


/*
	if(fork()==0) {
		kprintf("in child, exiting child\n");
	}
	else {
		kprintf("in parent\n");
        while(1);
	}*/
	while(1);
  return 0;
}
