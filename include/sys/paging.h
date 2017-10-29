#ifndef _PAGING_H
#define _PAGING_H
typedef struct {
	uint64_t starting_addr;
	uint64_t last_addr;
} smap_copy_t;

typedef struct {
	uint8_t is_avail;
}__attribute__((__packed__)) pg_desc_t;

pg_desc_t *free_list;

#endif
