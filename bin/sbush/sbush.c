
int main(int argc, char *argv[], char *envp[]) {
          __asm__("int $0x80\n\t");
        //kprintf("hi\n");
        while(1);
  return 0;
}
