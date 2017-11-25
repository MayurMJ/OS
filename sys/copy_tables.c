#include <sys/paging.h>
uint64_t create_table() {
	uint64_t *newPML4 = (uint64_t*) get_physical_free_page();
	uint64_t *temp = (uint64_t *)((uint64_t) newPML4 + (uint64_t)0xffffffff80000000 );
 	for(int i = 0; i < 512; i++) {
		temp[i] = PML4[i];
	}
	return (uint64_t) newPML4;
}

uint64_t copy_on_write() {
	uint64_t oldcr3;
	__asm__ __volatile__("movq %%cr3, %0\n\t"
			     :"=a"(oldcr3));
	uint64_t newcr3 = create_table();
	uint64_t *PML4_old = (uint64_t *)((uint64_t) oldcr3 + (uint64_t)0xffffffff80000000 );
	uint64_t *PML4_new = (uint64_t *)((uint64_t) newcr3 + (uint64_t)0xffffffff80000000 );
	for(int i = 0; i < 511; i++) {
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
							PDE_new[k] = (PDE_new[k] >> 12) << 12;
							PDE_new[k] |= (uint64_t) 0x805; 
						}
					}
				}
			}
			
		}
	}
	return newcr3;	
}
