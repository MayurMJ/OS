#include <sys/paging.h>
#include <sys/kprintf.h>

uint64_t create_table() {
	uint64_t *newPML4 = (uint64_t*) get_physical_free_page();
	uint64_t *temp = (uint64_t *)((uint64_t) newPML4 + (uint64_t)0xffffffff80000000 );
	uint64_t *PML4 = (uint64_t *)((uint64_t) PML4_kern + (uint64_t)0xffffffff80000000 );
 	for(int i = 0; i < 511; i++) {
		temp[i] = 0;
	}
	temp[511] = PML4[511];
	return (uint64_t) newPML4;
}

uint64_t copy_on_write() {
	uint64_t oldcr3;
	__asm__ __volatile__("movq %%cr3, %0\n\t"
			     :"=a"(oldcr3));
	uint64_t newcr3 = create_table();
	uint64_t *PML4_old = (uint64_t *)((uint64_t) oldcr3 + (uint64_t)0xffffffff80000000 );
	uint64_t *PML4_new = (uint64_t *)((uint64_t) newcr3 + (uint64_t)0xffffffff80000000 );
	int count = 0;
	for(int i = 0; i < 511; i++) {
		count++;
		PML4_new[i] = PML4_old[i];
		if(PML4_old[i] != 0) {
			PML4_new[i] = get_physical_free_page();
			uint64_t *PDTP_old = (uint64_t *)((((uint64_t) PML4_old[i] + (uint64_t)0xffffffff80000000) >> 12) <<12 );
			uint64_t *PDTP_new = (uint64_t *)((uint64_t) PML4_new[i] + (uint64_t)0xffffffff80000000 );	
			PML4_new[i] |= (uint64_t) 7;
			for(int j = 0; j < 512; j++) {
				PDTP_new[j] = PDTP_old[j];
				if(PDTP_old[j] != 0) {
					PDTP_new[j] = get_physical_free_page();			
					uint64_t *PDE_old = (uint64_t *)((((uint64_t) PDTP_old[j] + (uint64_t)0xffffffff80000000 )>>12) << 12);
					uint64_t *PDE_new = (uint64_t *)((uint64_t) PDTP_new[j] + (uint64_t)0xffffffff80000000 );
					PDTP_new[j] |= (uint64_t) 7;
					for(int k = 0; k < 512; k++) {
						PDE_new[k] = PDE_old[k];
						if(PDE_old[k] != 0) {
							PDE_new[k] = get_physical_free_page();
							//kprintf("count val %d, %d, %d %x\n",i,j,k, PDE_new[k]);
							uint64_t *PTE_old = (uint64_t *)((((uint64_t) PDE_old[k] + (uint64_t)0xffffffff80000000 )>>12) << 12);
							uint64_t *PTE_new = (uint64_t *)((uint64_t) PDE_new[k] + (uint64_t)0xffffffff80000000 );
							PDE_new[k] |= (uint64_t) 7;
							for(int l = 0; l < 512; l++) {
								PTE_new[l] = PTE_old[l];
								if(PTE_old[l] != 0) {
									//kprintf("count val %d, %d, %d %x %d %x\n",i,j,k, PDE_new[k],l,PTE_old[l]);
									PTE_new[l] = (PTE_new[l] >> 12) << 12;
									PTE_new[l] |= (uint64_t) 0x805;
									PTE_old[l] &= (uint64_t) 0xfffffffd;
									PTE_old[l] |= (uint64_t) 0x800;
									free_list[(((PTE_old[l]) >> 12 ) << 12) / 4096].ref_count++;
								}
							}
						}
					}
				}
			}
			
		}
	}
	reload_cr3();
	return newcr3;	
}

void delete_page_tables(uint64_t cr3) {
	uint64_t *PML4 = (uint64_t*) ((uint64_t) 0xffffffff80000000 + cr3);
	for(int i = 0; i < 511; i++) {
		if(PML4[i] != 0) {
			uint64_t *PDPTE = (uint64_t *)((((uint64_t) PML4[i] + (uint64_t)0xffffffff80000000) >> 12) <<12 );
			for(int j = 0; j < 512; j++ ) {
				if(PDPTE[j] != 0) {
					uint64_t *PDE = (uint64_t *)((((uint64_t) PDPTE[j] + (uint64_t)0xffffffff80000000) >> 12) << 12 );
					for(int k = 0; k < 512; k++) {
						if(PDE[k] != 0) {
							uint64_t *PTE = (uint64_t *)((((uint64_t) PDE[k] + (uint64_t)0xffffffff80000000) >> 12) << 12 );
							for(int l = 0; l < 512; l++) {
								if(PTE[l] != 0) {
									uint64_t temp = (PTE[l] >> 12) <<12;
									free_list[temp / 4096].ref_count--;
						                        if(free_list[temp / 4096].ref_count == 0) {
                                						free_physical_page((pg_desc_t*)((temp >> 12) <<12));
                       							 }	
								}
							}
							free_physical_page((pg_desc_t*)((PDE[k] >> 12) <<12));	
						}		
					}
					free_physical_page((pg_desc_t*)((PDPTE[j] >> 12) <<12));
				}
			}
			free_physical_page((pg_desc_t*)((PML4[i] >> 12) <<12));
		}
	//free_physical_page((pg_desc_t*)((cr3 >> 12) <<12));
	}
}
