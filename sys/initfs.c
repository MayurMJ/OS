#include <sys/elf64.h>
#include <sys/tarfs.h>
#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/kernel_threads.h>
#include <sys/paging.h>
#include <sys/kmemcpy.h>
#include <sys/string.h>
#include <sys/utils.h>
void parse_elf(char *fileName) {
        struct posix_header_ustar * header = (struct posix_header_ustar *)&_binary_tarfs_start;
        //kprintf("size of header %d",sizeof(struct posix_header_ustar));
        while(header<(struct posix_header_ustar *)&_binary_tarfs_end) {
                uint64_t size = octalToDecimal(stoi(header->size));
                if (size == 0) {
                        kprintf("\nFileName: %s\n", header->name);
                        if(header->name[0] == 0) break; 
			header++;
                }
                else {
                        kprintf("\nFileName: %s\n", header->name);
			size = (size%512==0) ? size +512: size + 512 + (512-size%512);
			header = (struct posix_header_ustar *) (((uint64_t)(header)) + size);
		}
	}
}
