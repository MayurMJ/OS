#include <sys/paging.h>
#include <sys/kprintf.h>
pg_desc_t *free_list_head;
extern char kernmem, physbase;

uint64_t get_physical_free_page () {
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

void free_physical_page( pg_desc_t *page){
  page->next=free_list_head;
  page->prev = NULL;
  page->is_avail = 1;
  free_list_head = page;
}

uint64_t setup_memory( void *physbase, void *physfree, smap_copy_t *smap_copy, int index) {
    int num_pages = 0;
    num_pages = (smap_copy[index-1].last_addr - smap_copy[0].starting_addr)/4096;
    uint64_t free_list_begin;
    if (((uint64_t)physfree & 0x0000000000000fff) == 0)
        free_list_begin = (uint64_t)(physfree);
    else
        free_list_begin = (((uint64_t)(physfree+4096)>>12)<<12);

    free_list = (pg_desc_t *)free_list_begin;

    uint64_t free_list_end;
    if (((free_list_begin + (num_pages * sizeof(pg_desc_t))) & 0x0000000000000fff) == 0)
        free_list_end = (free_list_begin + (num_pages * sizeof(pg_desc_t)));
    else
        free_list_end = (((free_list_begin + (num_pages * sizeof(pg_desc_t)))+4096)>>12)<<12;
    
   // mainTask = (Task *)(free_list_end + 0xffffffff80000000);
    free_list_end += 4096;
   // otherTask = (Task *)(free_list_end + 0xffffffff80000000);
    free_list_end += 4096;
    free_list_end += 4096; // since stack grows downward
    //otherTask->kstack = (uint64_t *)(free_list_end  + 0xffffffff80000000);
  // mark area between (kernmem+physbase) and (kernmem+physfree+space occupied by free_list) as occupied
  free_list[0].is_avail = 1;
  free_list[0].prev = NULL;
  free_list[0].next =(struct pg_desc *) ((uint64_t)0xffffffff80000000 + (uint64_t)(&free_list[1]));
  free_list[0].index = 0;
  int i=0;
  for (i=1; i < (num_pages-1) ; i++) {
        free_list[i].is_avail = 1;
        free_list[i].prev =(struct pg_desc *) ((uint64_t)0xffffffff80000000 + (uint64_t)(&free_list[i-1]));
        free_list[i].next =(struct pg_desc *) ((uint64_t)0xffffffff80000000 + (uint64_t)(&free_list[i+1]));
        free_list[i].index = i;
  }
  free_list[num_pages - 1].is_avail = 1;
  free_list[num_pages - 1].prev =(struct pg_desc *) ((uint64_t)0xffffffff80000000 + (uint64_t) &free_list[num_pages - 2]);
  free_list[num_pages - 1].next = NULL;
  free_list[num_pages - 1].index = num_pages - 1;

  free_list_head = (&free_list[1]);

  uint64_t x;
  // note free_list_begin and free_list_end hold vaddresses while smap_copy holds phys addresses

  uint64_t begin;
  if (((uint64_t)physbase & 0x0000000000000fff) == 0) // already aligned
        begin = (uint64_t)physbase;
  else
        begin = ((uint64_t)physbase >> 12) << 12;


  // kernel + free list area
  for (x=begin ; x < free_list_end+(520*4096); x+=4096) {
        free_list[x/4096].is_avail = 0; // it is not free
        if ((x/4096) == (num_pages-1)) {
                free_list[(x/4096)-1].next = NULL;
                free_list[x/4096].prev = NULL;
                free_list[x/4096].next = NULL;
        }
        else if ((x/4096) == 1) {
                free_list_head = &free_list[(x/4096)+1];
                free_list[(x/4096)+1].prev = NULL;
                free_list[x/4096].prev = NULL;
                free_list[x/4096].next = NULL;
        }
        else {
                free_list[(x/4096)-1].next =(struct pg_desc *) ((uint64_t)0xffffffff80000000 + (uint64_t) &free_list[(x/4096)+1]);
                free_list[(x/4096)+1].prev =(struct pg_desc *) ((uint64_t)0xffffffff80000000 + (uint64_t) &free_list[(x/4096)-1]);
                free_list[x/4096].prev = NULL;
                free_list[x/4096].next = NULL;
        }
        //kprintf("%d ",x/4096);
  }


    int j;
  // other areas where ram does not exist
  for (i=0; i<(index-1) ; i++) {
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
                else if (j == 1) {
                        free_list_head = &free_list[j+1];
                        free_list[j+1].prev = NULL;
                        free_list[j].prev = NULL;
                        free_list[j].next = NULL;
                }
                else {
                        free_list[j-1].next =(struct pg_desc *) ((uint64_t)0xffffffff80000000 + (uint64_t)&free_list[j+1]);
                        free_list[j+1].prev =(struct pg_desc *) ((uint64_t)0xffffffff80000000 + (uint64_t)&free_list[j-1]);
                        free_list[j].prev = NULL;
                        free_list[j].next = NULL;
                }
                //kprintf("%d ",j);
        }
  }
  free_list[free_list_end / 4096].is_avail = 0;
  free_list[(free_list_end / 4096) + 1].is_avail = 0;
  free_list_head = (struct pg_desc *)((uint64_t)0xffffffff80000000 + (uint64_t)free_list_head);
    return free_list_end;
}

void init_self_referencing(uint64_t free_list_end, uint64_t index) {
  PML4 =(uint64_t *) ((uint64_t)free_list_end);
  *PML4 = 0;
  PDE = (uint64_t *)((uint64_t)free_list_end+(uint64_t)4096);
  PTE1 = (uint64_t *)((uint64_t)free_list_end+(uint64_t)8192);
  uint64_t * PTE2 = (uint64_t *)((uint64_t)free_list_end+(uint64_t)12288);
  PML4[511] = ((uint64_t)free_list_end) | 3;
  PML4[510] = ((uint64_t)PDE) | 3; 
  for(int i = 0; i < 510; i++) {
    PML4[i] = 0;
  }
  PDE[0] = (uint64_t)PTE1;
  PDE[index] = (uint64_t)PTE2;

}

void map_memory_range(uint64_t start, uint64_t end, uint64_t map_index) {
  //uint64_t offset = (uint64_t) (map_index *512);
  uint64_t *PTE = (uint64_t *)PDE[map_index];
  //PML4[map_index] = (uint64_t)PTE;
  //PML4[map_index] = PML4[map_index] | 3;
  uint64_t temp_addr = (uint64_t)(0xffffffff80000000 + start);
  temp_addr = temp_addr >> 12;
  uint64_t temp = 0x1ff;
  uint64_t ind = temp & temp_addr;
  for(int i = 0; i < ind; ++i) {
    PTE[i] = 0;
  }
  for(uint64_t x = (uint64_t)start; x < (uint64_t) end; x += 4096) {
    PTE[ind] = x | 3;
    ind++;
  }
  for(int i = ind; i < 512; i++) {
    PTE[i] = 0;
  }
}
uint64_t get_free_page(uint64_t flags) {
  uint64_t phy_addr = get_physical_free_page();
//  uint64_t virt_addr = 0xffffffff80000000 + phy_addr;
  uint64_t virt_addr;
   if(flags == 7)
	virt_addr= 4096 + phy_addr;
   else
	virt_addr = 0xffffffff80000000 + phy_addr;

  uint64_t *VA = (uint64_t*)(0xffffffff80000000 + 0xa0000);
  uint64_t PMLframe = (virt_addr >> 38) & (uint64_t) 0x1ff;
  uint64_t PDPTEindex = (virt_addr >> 29) & (uint64_t) 0x1ff;
  uint64_t PDEindex = (virt_addr >> 20) & (uint64_t) 0x1ff;
  uint64_t PTEindex = (virt_addr >> 11) & (uint64_t) 0x1ff;
  if(PML4[PMLframe] == 0) {
    uint64_t *PDPTE = (uint64_t *)get_physical_free_page();
    uint64_t *PDE = (uint64_t *)get_physical_free_page();
    uint64_t *PTE = (uint64_t *)get_physical_free_page();
    PML4[PMLframe] = (uint64_t) PDPTE | 3;

    PTE1[160] = 0;
    PTE1[160] = (uint64_t) PDPTE | 3;
    for(int i = 0; i < 512; i++) {
     VA[i] = 0;
    }
    VA[PDPTEindex] = (uint64_t) PDE ;
    VA[PDPTEindex] = (uint64_t) VA[PDPTEindex] |(uint64_t) flags;
    
    
    PTE1[160] = 0;
    PTE1[160] = (uint64_t) PDE | 7;
    for(int i = 0; i < 512; i++) {
     VA[i] = 0;
    }
    VA[PDEindex] = (uint64_t) PTE;
    VA[PDEindex] = (uint64_t)VA[PDPTEindex] |(uint64_t) flags;

    PTE1[160] = 0;
    PTE1[160] = (uint64_t) PTE | 7;
    for(int i = 0; i < 512; i++) {
     VA[i] = 0;
    }
    VA[PTEindex] = (uint64_t) phy_addr;
    VA[PTEindex] = (uint64_t)VA[PDPTEindex] |(uint64_t) flags;
    return virt_addr;
  }
  uint64_t x = (uint64_t)0xffffffff80000000 + (uint64_t) PML4[PMLframe];
  x = x & 0xfffffffffffffffc; 
  uint64_t *PTE = (uint64_t *)x;
  uint64_t PTEframe = (virt_addr >> 12) & (uint64_t) 0x1ff;
  PTE[PTEframe] = phy_addr | flags;
  return virt_addr; 
}
