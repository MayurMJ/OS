#include <stdlib.h>

static int _main(int, char* argv[]);

void _start(void) {
  // call main() and exit() here
  register int *sp  __asm("rsp");


  _main(*(sp + 2), (char **)(sp + 4));
  //main(c,NULL,NULL);
   __asm("movl $1,%eax;"
        "xorl %ebx,%ebx;"
        "int  $0x80"
    );
}
static int _main(int argc, char *argv[]) {
	main(argc, argv, argc + argv + 1);
        return 0;
}
