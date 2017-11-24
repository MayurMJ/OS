
        int i = 0;
int main(int argc, char *argv[], char *envp[]) {
        for(i = 0; i < 10; i++) {  
        __asm__("int $0x80\n\t");}
        //kprintf("hi\n");
        while(1);
  return 0;
}
