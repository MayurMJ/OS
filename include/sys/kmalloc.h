#ifndef _KMALLOC_H_
#define _KMALLOC_H_

#include <sys/defs.h>
#define slab_bufctl(slabp)  ((kmem_bufctl_t *)(((slab_t*)slabp)+1))

typedef unsigned int kmem_bufctl_t; 
typedef struct slab_s { 
    struct kmem_cache_t * curr_cache;
    struct slab_t *     next;           /* linkage into free/full/partial lists */
    unsigned long       colouroff; /* offset in bytes of objects into first page of slab */ 
    void                *s_mem;             /* pointer to where the objects start */
    unsigned int        inuse;          /* num of objs allocated in the slab */
    kmem_bufctl_t       free;       /*index of the next free object */
} slab_t;

typedef struct kmem_cache_t {
    struct slab_t *    slabs_full;
    struct slab_t *   slabs_partial;
    unsigned int        objsize;  /*The size of each object in this cache.*/
    unsigned int        num;  //The number of objects stored on each slab.   
} kmem_cache_t;

void * virt_to_page(void *objp);
void * alloc_obj(kmem_cache_t *cachep, slab_t *slabp);
void free_obj(void *objp);

#endif
