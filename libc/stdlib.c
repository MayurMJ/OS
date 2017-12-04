#include <sys/defs.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/defs.h>
void exit(int ret) {
	uint64_t syscall_code = 60;
//	uint64_t retval= ret;
	uint64_t result;
	__asm__ __volatile("int $0x80\n\t"
			   :"=a"(result)
			   :"a"(syscall_code), "D"(ret));
}

void *malloc(size_t size) {
	static m_header *start = NULL;
	static m_header *end = NULL;
	size_t alloc_size = (size + sizeof(m_header) + 15 ) & ~(15);
	alloc_size = alloc_size << 1;
	uint64_t *block = NULL;//find_first_fit(alloc_size);
	if(block == NULL) {
		uint64_t temp_block = ret_brk();
		uint64_t end_block = (uint64_t)temp_block + alloc_size;
		int status = brk((void*) end_block);
		if(status == -1) {
		//	puts("Not Possible to allocate More memory");
		}
		else {
			m_header *new_header = (m_header*)temp_block;
			new_header->size = alloc_size;
			new_header->next = NULL;
			if(start == NULL) {
				start = new_header;
				end = new_header;
			}
			else {
				end->next = new_header;
				end = end->next;
			}
			return (void*)(temp_block + sizeof(m_header));
		}
	}
	else {
		return NULL;
	}
	return NULL;
}
