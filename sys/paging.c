#include <sys/paging.h>
#include <sys/kprintf.h>

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

void free__physical_page( pg_desc_t *page){
  page->next=free_list_head;
  page->prev = NULL;
  page->is_avail = 1;
  free_list_head = page;
}
