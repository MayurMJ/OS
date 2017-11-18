#include <sys/kmalloc.h>
#include <sys/paging.h>

void * virt_to_page(void *objp) {
  uint64_t addr = (uint64_t)objp;
  addr = addr & 0xfffffffffffff000;
  return (void *)addr;
}

void * alloc_obj(kmem_cache_t *cachep, slab_t *slabp) {
  void * objp;
  
  objp = slabp->s_mem + slabp->free*cachep->objsize;
  slabp->free=slab_bufctl(slabp)[slabp->free];

  /*check now to see if the current slab is full i.e. new slabp->free points to BUFCTL_END. In that case, move this slab to full slabs and maybe allocate a new slab new to point it to partial slab list */

  return objp;
}

void free_obj(void *objp) {

  slab_t *slabp = (slab_t *) virt_to_page(objp);
  kmem_cache_t *cachep = (kmem_cache_t *) slabp->curr_cache;
  

  /* get the index of this object offset from s_mem */
  unsigned int obj_index = (objp-slabp->s_mem)/cachep->objsize;
  slab_bufctl(slabp)[obj_index] = slabp->free;
  slabp->free = obj_index;
 
  /*maybe check this slab's number of objs allocated and if the counts gets to zero, free this page/slab*/

}

uint64_t pow(uint64_t num, uint64_t power) {
  if(num==0||num==1)
    return num;
  uint64_t res = 1;
  for(int i=0;i<power;i++)
    res = res * num;
  return res;
}

void init_kmalloc() {
  void * kmem_cache = (void *)get_free_page(3);
  cache_cache = (kmem_cache_t *) kmem_cache;
  for(int i = 3; i<NUM_CACHES+3; i++) {
    cache_cache[i].slabs_full = NULL;
    cache_cache[i].slabs_partial = NULL;
    cache_cache[i].objsize = pow(2,i);
    cache_cache[i].num = 0; //to be updated
  }
}
