#ifndef _PAGING_H
#define _PAGING_H
typedef struct {
	uint64_t starting_addr;
	uint64_t last_addr;
} smap_copy_t;

typedef struct pg_desc {
	uint8_t is_avail;
	uint64_t index;
	struct pg_desc *prev;
	struct pg_desc *next; 
}__attribute__((__packed__)) pg_desc_t;

pg_desc_t *free_list;

uint64_t get_physical_free_page ();
void free_physical_page( pg_desc_t *page);
void free_page(void *addr);
uint64_t get_free_page();

#endif
