#include <sys/defs.h>
#include <sys/gdt.h>
#include <sys/kprintf.h>
#include <sys/tarfs.h>
#include <sys/ahci.h>
#include <sys/idt.h>
#include <sys/pic.h>
#include <sys/kernel_threads.h>
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
pg_desc_t *free_list_head;

Task *runningTask;
void f1() {
        kprintf("f1 1\n");
        kprintf("f1 2\n");
        kprintf("f1 3\n");
        //yield();
}

void createTask(Task *task, void (*main)(), Task *otherTask) {
    task->regs.rax = 0;
    task->regs.rbx = 0;
    task->regs.rcx = 0;
    task->regs.rdx = 0;
    task->regs.rsi = 0;
    task->regs.rdi = 0;
    task->regs.rflags = otherTask->regs.rflags;
    task->regs.rip = (uint64_t) main;
    task->regs.cr3 = (uint64_t) otherTask->regs.cr3;
    //task->regs.esp = (uint32_t) allocPage() + 0x1000; // Not implemented here
    task->next = 0;
}

void initTasking(Task *mainTask, Task *otherTask) {
	__asm__ __volatile__("movq %%cr3, %0\n\t"
                    	     :"=a"(mainTask->regs.cr3));
	__asm__ __volatile__("push %%RFLAGS \n\t"
			     "movq (%%rsp), %%rax\n\t"
			     "movq %%rax, %0\n\t"
			     "pop %%rflags\n\t"
			     :"=m"(mainTask->regs.rflags)::"%rax");
	
	createTask(otherTask, f1, mainTask);
    	mainTask->next = otherTask;
    	otherTask->next = mainTask;
 
    	runningTask = mainTask;
}

uint64_t get_free_page () {
  if(free_list_head == NULL)
    return 0;
  if(free_list_head->is_avail==0) {
    kprintf("ERROR: trying to allocate a non avaialable page\n");
    return 0;
  }
  uint64_t addr = (uint64_t) (free_list_head->index * 4096);
  pg_desc_t * temp = free_list_head;
  free_list_head = free_list_head->next;
  temp->next = NULL;
  temp->prev = NULL;
  temp->is_avail = 0;
  return addr;
}

void __free_page( pg_desc_t *page){
  page->next=free_list_head;
  page->prev = NULL;
  page->is_avail = 1;
  free_list_head = page;
}
void free_page(void *addr);
  
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

      kprintf("Available Physical Memory [%p-%p]\n",  smap_copy[smap_copy_index].starting_addr, smap_copy[smap_copy_index].last_addr );
      
      smap_copy_index++;
    }
  }
  
//  kprintf("physfree %p physbase %p\n", (uint64_t)physfree, (uint64_t)physbase);
  kprintf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
  num_pages = (smap_copy[smap_copy_index-1].last_addr - smap_copy[0].starting_addr)/4096;
  kprintf("Num Pages %d\n", num_pages);
  uint64_t free_list_begin;
  if (((uint64_t)physfree & 0x0000000000000fff) == 0)
	free_list_begin = (uint64_t)physfree;
  else
   	free_list_begin = ((((uint64_t)physfree+4096)>>12)<<12);

  free_list = (pg_desc_t *)free_list_begin;
  

  uint64_t free_list_end;
  if (((free_list_begin + (num_pages * sizeof(pg_desc_t))) & 0x0000000000000fff) == 0)
	free_list_end = (free_list_begin + (num_pages * sizeof(pg_desc_t)));
  else
   	free_list_end = (((free_list_begin + (num_pages * sizeof(pg_desc_t)))+4096)>>12)<<12;

  // --------------------------------------
  Task *mainTask = (Task *)free_list_end;
  free_list_end += 4096;
  Task *otherTask = (Task *)free_list_end;
  free_list_end += 4096;
  otherTask->kstack = (uint64_t *)free_list_end;
  free_list_end += 4096;
  // --------------------------------------
  // mark area between (kernmem+physbase) and (kernmem+physfree+space occupied by free_list) as occupied
  free_list[1].is_avail = 1;
  free_list[1].prev = NULL;
  free_list[1].next = &free_list[2];
  free_list[1].index = 0;

  for (i=2; i < (num_pages-1) ; i++) {
	free_list[i].is_avail = 1;
	free_list[i].prev = &free_list[i-1];
	free_list[i].next = &free_list[i+1];
	free_list[i].index = i;
  }
  free_list[num_pages - 1].is_avail = 1;
  free_list[num_pages - 1].prev = &free_list[num_pages - 2];
  free_list[num_pages - 1].next = NULL;
  free_list[num_pages - 1].index = num_pages - 1; 

  free_list_head = &free_list[1];

  uint64_t x;
  // note free_list_begin and free_list_end hold vaddresses while smap_copy holds phys addresses

  uint64_t begin;
  if (((uint64_t)physbase & 0x0000000000000fff) == 0) // already aligned
	begin = (uint64_t)physbase;
  else
	begin = ((uint64_t)physbase >> 12) << 12;

  // kernel + free list area
  for (x=begin ; x < free_list_end; x+=4096) {
	free_list[x/4096].is_avail = 0; // it is not free
	if ((x/4096) == (num_pages-1)) {
		free_list[(x/4096)-1].next = NULL;
		free_list[x/4096].prev = NULL;
		free_list[x/4096].next = NULL;
	}
	else if ((x/4096) == 0) {
		free_list_head = &free_list[(x/4096)+1];
		free_list[(x/4096)+1].prev = NULL;
		free_list[x/4096].prev = NULL;
                free_list[x/4096].next = NULL;
	}
	else {
		free_list[(x/4096)-1].next = &free_list[(x/4096)+1];
		free_list[(x/4096)+1].prev = &free_list[(x/4096)-1];
		free_list[x/4096].prev = NULL;
                free_list[x/4096].next = NULL;
	}
	//kprintf("%d ",x/4096);
  } 
  //kprintf("\n");
  int j; 
  // other areas where ram does not exist
  for (i=0; i<(smap_copy_index-1) ; i++) {
	int first_page, second_page;
	first_page = smap_copy[i].last_addr/ 4096;
	if ((smap_copy[i+1].starting_addr & 0x0000000000000fff) == 0) // already aligned
		second_page = smap_copy[i+1].starting_addr / 4096;
	else
		second_page = (smap_copy[i+1].starting_addr / 4096) + 1;
	for (j = first_page; j < second_page; j++) {
		free_list[j].is_avail = 0; // it is not free
		if (j == (num_pages-1)) {
         	       	free_list[j-1].next = NULL;
                	free_list[j].prev = NULL;
               		free_list[j].next = NULL;
        	}
        	else if (j == 0) {
                	free_list_head = &free_list[j+1];
                	free_list[j+1].prev = NULL;
                	free_list[j].prev = NULL;
                	free_list[j].next = NULL;
        	}
        	else {
                	free_list[j-1].next = &free_list[j+1];
                	free_list[j+1].prev = &free_list[j-1];
                	free_list[j].prev = NULL;
                	free_list[j].next = NULL;
        	}
		//kprintf("%d ",j);
	}
  }
  //kprintf("random print %d\n",free_list_head->is_avail);
  uint64_t cr3val;
  __asm __volatile("movq %%cr3, %0\n\t"
                    :"=a"(cr3val));
  kprintf("\nValue of cr3: %x", cr3val);
/*  __asm __volatile("movq %%ia32_efer, %0\n\t"
                    :"=a"(cr3val));
    kprintf("\nValue of efer: %x", cr3val);*/
    //free_list_end += 4096;
  free_list[free_list_end / 4096].is_avail = 0;
  free_list[(free_list_end / 4096) + 1].is_avail = 0;
  uint64_t *PML4 =(uint64_t *) ((uint64_t)free_list_end);
  //uint64_t *PML4 = (uint64_t *) PML;
  *PML4 = 0;
  PML4[511] = ((uint64_t)free_list_end) | 7;
  PML4[510] = ((uint64_t)free_list_end) | 7;
  uint64_t *PTE = (uint64_t *)PML4 + 512;
  PML4[1] = (uint64_t)PTE;
  PML4[1] = PML4[1] | 7;
  uint64_t temp_addr = (uint64_t)(0xffffffff80000000+physbase);
  temp_addr = temp_addr >> 12;
  uint64_t temp = 0x1ff;
  uint64_t ind = temp & temp_addr;
  kprintf("\nindex: %d", ind);
  for(uint64_t x = (uint64_t)physbase; x < (uint64_t) physfree; x += 4096) {
    PTE[ind] = x | 7;
    ind++;
  }
  for(int i = ind; i < 512; i++) {
    PTE[i] = 0;
  }
  for(int i = 2; i < 510; i++) {
    PML4[i] = 0;
  }
  /*remap the 640K-1M region with direct one to one mapping from virtual to physical*/
  PML4[0] = (uint64_t)PML4|7;
  ind = 160;
  uint64_t vidstart = 0xa0000;
  uint64_t vidend = 0x100000;
  for(x = (uint64_t) vidstart; x < (uint64_t) vidend; x += 4096) {
    PML4[ind] = x | 7;
    ind++;
  }

  cr3val = free_list_end;
  __asm __volatile("movq %0, %%cr3\n\t"
                    :
                    :"a"(cr3val));

  
  kprintf("\nTest Print after reclocation of CR3\n");
  // ------------------------------------------------
  initTasking(mainTask, otherTask);
  // ------------------------------------------------
 // init_idt();
 // program_pic();  
 //__asm__ __volatile("sti");
 
  //kprintf("physfree %p physbase %p\n", (uint64_t)physfree, (uint64_t)physbase);
  
  //hba_port_t* port = enumerate_pci();
  //if (port == NULL) kprintf("nothing found\n");
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
