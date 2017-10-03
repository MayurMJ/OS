#include <sys/defs.h>
#include <sys/gdt.h>
#include <sys/kprintf.h>
#include <sys/tarfs.h>
#include <sys/ahci.h>
#include <sys/idt.h>
#include <sys/pic.h>
#include <sys/pci.h>
#include <sys/kmemcpy.h>

#define INITIAL_STACK_SIZE 4096
uint8_t initial_stack[INITIAL_STACK_SIZE]__attribute__((aligned(16)));
uint32_t* loader_stack;
extern char kernmem, physbase;

void start(uint32_t *modulep, void *physbase, void *physfree)
{
//  int i = 0;
  struct smap_t {
    uint64_t base, length;
    uint32_t type;
  }__attribute__((packed)) *smap;
  while(modulep[0] != 0x9001) modulep += modulep[1]+2;
  for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
    if (smap->type == 1 /* memory */ && smap->length != 0) {
      kprintf("Available Physical Memory [%p-%p]\n", smap->base, smap->base + smap->length);
    }
  }
  kprintf("physfree %p\n", (uint64_t)physfree);
  kprintf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
  /*
  kprintf("doesnt work\rnope it doesnt\rstill doesnt work\rit works\n");
  kprintf("%x\n",0xffffffff);
  kprintf("%x\n",0xfffffff);
  kprintf("%x\n",-2147483648);
  kprintf("%x\n",2147483647);
  kprintf("%x\n",1);
  kprintf("%x\n",4294967295);
  kprintf("%x\n",10);
  */
  init_idt();
  program_pic();
  __asm__ __volatile("sti");
 // __asm__ __volatile("int  $32");
 // __asm__ __volatile("int  $32");
 // __asm__ __volatile("int  $32");
  hba_port_t* port = enumerate_pci();
  if (port == NULL) kprintf("nothing found\n");
 
  uint64_t buf = (uint64_t)0x3ff9c000;// + 1024 + 256 + 928 *32;

  memset((void *)buf,0,409600); 
	
 uint64_t tmpbuf = buf;
  int j;

  //kprintf("\nbefore kmemset %d",*((uint8_t *)(buf)));
  for(j=3;j < 103;j++) {
	memset((uint8_t *)tmpbuf,j,4096);
	tmpbuf += 4096;
  }

  //kprintf("\nbefore write %d",*((uint8_t *)(buf)));
 /* int sectorIndex = 0;
  for(sectorIndex = 0; sectorIndex < 5; sectorIndex++) {
	kprintf("\n%d WRITE tfd %x ssts %x sctl %x",sectorIndex,port->tfd,port->ssts, port->sctl);
    	for(int i = 0; i < 1000000; i++);
    	for(int i = 0; i < 1000000; i++);
	write(port, sectorIndex*8, 0, 8, buf + (sectorIndex *8 * 512));*/
// }

  //kprintf("\naftre write buf is %d",*((uint8_t *)(buf)));
 
	//int sectorIndex = 0;
//memset((uint8_t *)buf,0,409600);
/*  for(sectorIndex = 0; sectorIndex < 5; sectorIndex++) {
        kprintf("\n%d tfd %x ssts %x sctl %x",sectorIndex,port->tfd,port->ssts, port->sctl);
         read(port, sectorIndex*8, 0, 8, buf + (sectorIndex *8 * 512));
	kprintf("\nread returned %d\n",*((uint8_t *)(buf + (sectorIndex *8 * 512))));
 }
  //kprintf("\nafter memset 0 is %d\n",*((uint8_t *)(buf)));
//  for(i = 0; i < 100; i++) {
//  	ret = read(port, 0, 0, 1, buf);
//  	ret = read(port, 1, 0, 1, buf);
//  	ret = read(port, 2, 0, 1, buf);
//	kprintf("write returned %d \n", ret);

	//for(j=0;j<2;j++)
  	//	kprintf("%d ",*((uint8_t *)(buf + j)));
	//for(j=510;j<512;j++)
	//	kprintf("%d ",*((uint8_t *)(buf + j)));

	//kprintf("\n");
 // }

 // kprintf("\naftre read %d",*((uint8_t *)(buf)));
 // kprintf("\naftre read + 100 %d",*((uint8_t *)(buf+510)));
  //kprintf("\naftre read + 1023 %d %d %d",*((uint8_t *)(buf+1023)),*((uint8_t *)(buf+1024)),*((uint8_t *)(buf+1024)));
//  */
  //kprintf("\n%sWe are here", buf);
  while(1);
}

void boot(void)
{
  // note: function changes rsp, local stack variables can't be practically used
  register char /**temp1,*/ *temp2;

  for(temp2 = (char*)0xb8001; temp2 < (char*)0xb8000+160*25; temp2 += 2) *temp2 = 7 /* white */;
  __asm__ volatile (
    "cli;"
    "movq %%rsp, %0;"
    "movq %1, %%rsp;"
    :"=g"(loader_stack)
    :"r"(&initial_stack[INITIAL_STACK_SIZE])
  );
  init_gdt();
  start(
    (uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
    (uint64_t*)&physbase,
    (uint64_t*)(uint64_t)loader_stack[4]
  );
 /* for(
    temp1 = "!!!!! start() returned !!!!!", temp2 = (char*)0xb8000;
    *temp1;
    temp1 += 1, temp2 += 2
  ) *temp2 = *temp1;*/
  while(1) __asm__ volatile ("hlt");
}
