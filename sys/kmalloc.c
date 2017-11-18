#include <sys/kmalloc.h>

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
