#include <stdlib.h>

//static int _main(int, char* argv[]);

void _start(void) {
  /*
  // call main() and exit() here
  register int *sp  __asm("rsp");


  _main(*(sp + 2), (char **)(sp + 4));
  //main(c,NULL,NULL);
   __asm("movl $1,%eax;"
        "xorl %ebx,%ebx;"
        "int  $0x80"
    );
   */
   	int argc, status;
	char **argv, **envp;
	uint64_t tos;
	__asm__ __volatile__("mov %%rsp, %0"
			     :"=a"(tos)
			     :/* no input op */
			     :/* no output op */);
	argc = *((int *)(tos + 8));
	argv = ((char **)(tos + 16));
	envp = ((char **)(tos + 24 + (argc * 8)));
	status = main(argc, argv, envp);
	/* QUESTION: any clean up required before passing to exit ?? */
	exit(status);	
}
/*
static int _main(int argc, char *argv[]) {
  main(argc, argv, argc + argv + 1);
  return 0;
}
*/
