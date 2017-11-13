#ifndef _PAGING_H
#define _PAGING_H
#include <sys/defs.h>
typedef struct {
	uint64_t starting_addr;
	uint64_t last_addr;
} smap_copy_t;

typedef struct pg_desc {
	uint8_t is_avail;
	uint64_t index;
	uint64_t count;
	struct pg_desc *prev;
	struct pg_desc *next; 
}__attribute__((__packed__)) pg_desc_t;

pg_desc_t *free_list;

extern pg_desc_t *free_list_head;
uint64_t get_physical_free_page ();
void free_physical_page( pg_desc_t *page);
void free_page(void *addr);
uint64_t get_free_page();

#endif
