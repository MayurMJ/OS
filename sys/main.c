#include <sys/defs.h>
#include <sys/gdt.h>
#include <sys/kprintf.h>
#include <sys/tarfs.h>
#include <sys/ahci.h>
#include <sys/idt.h>
#include <sys/pic.h>
/*
deleted pci.c and pci.h due to reduction in available memory
#include <sys/pci.h>
*/
#include <sys/paging.h>
#include <sys/kmemcpy.h>

#define INITIAL_STACK_SIZE 4096
uint8_t initial_stack[INITIAL_STACK_SIZE]__attribute__((aligned(16)));
uint32_t* loader_stack;
extern char kernmem, physbase;

void start(uint32_t *modulep, void *physbase, void *physfree)
{
  struct smap_t {
    uint64_t base, length;
    uint32_t type;
  }__attribute__((packed)) *smap;

  int num_pages = 0;
  smap_copy_t smap_copy[10];
  int smap_copy_index = 0;
  int i;

  while(modulep[0] != 0x9001) modulep += modulep[1]+2;
  for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
    if (smap->type == 1 /* memory */ && smap->length != 0) {

      smap_copy[smap_copy_index].starting_addr = smap->base;
      smap_copy[smap_copy_index].last_addr = smap->base + smap->length;

      kprintf("Available Physical Memory [%p-%p]\n", smap->base, smap->base + smap->length);
      
      smap_copy_index++;
    }
  }
  
//  kprintf("physfree %p physbase %p\n", (uint64_t)physfree, (uint64_t)physbase);
  kprintf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
//  init_idt();
//  program_pic();
//  kprintf("physfree %p physbase %p\n", (uint64_t)physfree, (uint64_t)physbase);
  for (i=0; i < smap_copy_index ; i++) {
	uint64_t align_start_addr, align_end_addr;
	if ((smap_copy[i].starting_addr & 0x0000000000000fff) == 0) {
		// already aligned
		align_start_addr = smap_copy[i].starting_addr;
	}
	else {
		align_start_addr = ((smap_copy[i].starting_addr+4096) >> 12) << 12;
	}

	if ((smap_copy[i].last_addr & 0x0000000000000fff) == 0) {
                // already aligned
                align_end_addr = smap_copy[i].last_addr;
        }
        else {
                align_end_addr = (smap_copy[i].last_addr >> 12) << 12;
        }
  	num_pages += (align_end_addr - align_start_addr)/4096;
  }
  uint64_t *temp_free_list = (uint64_t *)(0xffffffff80000000 + physfree);
  free_list = (pg_desc_t *)temp_free_list;
  // mark area between (kernmem+physbase) and (kernmem+physfree+space occupied by free_list) as occupied
  

  __asm__ __volatile("sti");
  kprintf("physfree %p physbase %p\n", (uint64_t)physfree, (uint64_t)physbase);
  /*
  hba_port_t* port = enumerate_pci();
  if (port == NULL) kprintf("nothing found\n");
  */
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
