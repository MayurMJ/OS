#ifndef _PAGING_H
#define _PAGING_H
#include <sys/defs.h>

#define PAGE_SIZE	4096
uint64_t put_page_mapping(uint64_t flags, uint64_t virt_addr, uint64_t cr3val);
typedef struct {
	uint64_t starting_addr;
	uint64_t last_addr;
} smap_copy_t;

typedef struct pg_desc {
	uint8_t is_avail;
	uint64_t index;
	uint64_t count;
	uint64_t ref_count;
	struct pg_desc *prev;
	struct pg_desc *next; 
}__attribute__((__packed__)) pg_desc_t;

pg_desc_t *free_list;
uint64_t *PML4;
uint64_t *PDTP;
uint64_t *PDE;
uint64_t *PTE1;
extern pg_desc_t *free_list_head;

uint64_t *kstack;

uint64_t get_physical_free_page ();
void free_physical_page( pg_desc_t *page);
void free_page(void *addr);
uint64_t get_free_page(uint64_t);
uint64_t setup_memory( void *physbase, void *physfree, smap_copy_t *smap_copy, int index);
void init_self_referencing(uint64_t free_list_end, uint64_t index);
void map_memory_range(uint64_t start, uint64_t end, uint64_t ind);
uint64_t walk_pml4_get_address(uint64_t virt_addr, uint64_t cr3val);
void free_old_page_tables(uint64_t addr);
#endif
