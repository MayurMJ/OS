#include <sys/kmalloc.h>
#include <sys/paging.h>
#include <sys/kprintf.h>
#include <sys/kmemcpy.h>

void * virt_to_page(void *objp) {
  uint64_t addr = (uint64_t)objp;
  addr = addr & 0xfffffffffffff000;
  return (void *)addr;
}

void * alloc_obj(kmem_cache_t *cache, slab_t *slabp) {
  void * objp;
  
  objp = slabp->s_mem + slabp->free*cache->objsize;
  slabp->free=slab_bufctl(slabp)[slabp->free];

  /*check now to see if the current slab is full i.e. new slabp->free points to BUFCTL_END. In that case, move this slab to full slabs and maybe allocate a new slab new to point it to partial slab list */
  if(slabp->free == BUFCTL_END) {
      //move this slab to cache->slabs_full 
      //----- TBD ------    
      if(slabp->next == NULL)
        cache->slabs_partial = alloc_slab(cache);
      else
        cache->slabs_partial = slabp->next;

      slab_t *temp = cache->slabs_full; 
      while(temp->next != NULL)  {
	temp = temp->next;
      }
      temp->next = slabp;
      slabp->next = NULL;
  }

  return objp;
}
//TODO: recheck this
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
  for(int i = 0; i<NUM_CACHES; i++) {
    (cache_cache+i)->slabs_full = NULL;
    (cache_cache+i)->slabs_partial = NULL;
    (cache_cache+i)->objsize = pow(2,i+3);
    (cache_cache+i)->num = objs_in_slab((cache_cache+i)->objsize); //to be updated
  }
}

kmem_cache_t * get_fit_cache(size_t size) {

  kmem_cache_t * cache = (kmem_cache_t *) cache_cache;
  while(cache <= (cache+NUM_CACHES)) {
    // find the smallest cache that can fit in the requested size //
    if(cache->objsize >= size) {
      return cache;
    }
    cache++;
  }
  return cache;
}


/* returns number of objs in cache of size objsize

Numbers are:

objsize = 8 num = 336
objsize = 16 num = 201
objsize = 32 num = 112
objsize = 64 num = 59
objsize = 128 num = 30
objsize = 256 num = 15
objsize = 512 num = 7
objsize = 1024 num = 3                          
*/
uint32_t objs_in_slab(unsigned int objsize) {
    uint32_t num = (PAGE_SIZE - sizeof(slab_t))/(sizeof(kmem_bufctl_t) + objsize); //4096 = (slab_t)+(kmem_bufctl_t*num)+(objsize*num); num = 4096-slab_t/(objsize+kmem_buf_ctl);
    return num;
}


// allocate a new slab for the current cache
slab_t * alloc_slab(kmem_cache_t * cache) {

    slab_t * slab = (slab_t *) get_free_page(3);
    if(!slab) return NULL;

    slab->curr_cache = cache;
    slab->next = NULL; //since there doesn't exist any other partial slab. (revisit later)
    slab->inuse = 0;   // 0 used objects in the slab
    slab->free = 0;    //index of first free object 
    uint32_t num_objs = objs_in_slab(cache->objsize); //number of objs that will fit in this slab
    
    //init the array list to point to next element
    for(int i = 0;i<num_objs-1;i++)
        slab_bufctl(slab)[i]=i+1;
    slab_bufctl(slab)[num_objs-1] = BUFCTL_END; //last element

    slab->s_mem = (void *) ( ((uint64_t)slab) + sizeof(slab_t) + (num_objs*sizeof(kmem_bufctl_t)) );
    return slab;
}


//  return the pointer to the partial slab of the current cache
slab_t * get_partial_slab(kmem_cache_t * cache) {

  // partial slab of the cache is not allocated //
  if(cache->slabs_partial == NULL) {

    // allocate a new slab for the given cache
    cache->slabs_partial = alloc_slab(cache);
  }

  //return the partial slab of the cache
  return cache->slabs_partial;
    
}

void* kmalloc(size_t size) {

  if(!size) return NULL;
  if(size>1024) return (void*)get_free_page(3);
 
  kmem_cache_t * cache = get_fit_cache(size);
  if (!cache) return NULL;

  slab_t * slab = get_partial_slab(cache);
  if(!slab) return NULL;

  return alloc_obj(cache,slab);
}

void kfree(uint64_t *virt_addr) {
  slab_t *slab = virt_to_page((void*) virt_addr);
  
  // If slab is in the full list move it to the end of partial list 
  // TODO may be make it more efficient with a doubly linked list
  if(slab->free == BUFCTL_END) {
    slab_t *temp1 = slab->curr_cache->slabs_full;
    while(temp1->next != slab) {
      temp1 = temp1->next;
    }
    temp1->next = temp1->next->next;
  }

  if(slab->free == BUFCTL_END) {
    slab_t *temp = slab->curr_cache->slabs_partial;
    while(temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = slab;
    slab->next = NULL;
  }
  // memset obj to 0  
  memset((char *)virt_addr,0,slab->curr_cache->objsize); 
 
  kmem_bufctl_t index = ((uint64_t)virt_addr - (uint64_t) slab->s_mem) /  slab->curr_cache->objsize;
  kmem_bufctl_t temp = slab_bufctl(slab)[index];
  slab_bufctl(slab)[index] = slab->free; 
  slab->free = temp;
}
   
