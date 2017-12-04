#include <sys/defs.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
void exit(int ret) {
	uint64_t syscall_code = 60;
//	uint64_t retval= ret;
	uint64_t result;
	__asm__ __volatile("int $0x80\n\t"
			   :"=a"(result)
			   :"a"(syscall_code), "D"(ret));
}

void *find_first_fit(m_header *start, size_t alloc_size) {
	if(start == NULL) return NULL;
	m_header *temp = start;
	while(temp != NULL) {
		if(temp->size >= alloc_size && temp->available == 1) 
			return temp;
		temp = temp->next;
	}
	return NULL;
}

void *malloc(size_t size) {
	static m_header *start = NULL;
	static m_header *end = NULL;
	size_t alloc_size = (size + sizeof(m_header) + 15 ) & ~(15);
	alloc_size = alloc_size << 1;
	m_header *block = (m_header*)find_first_fit(start , alloc_size);
	if(block != NULL) {
		block->size = alloc_size;
		block->available = 0;
		return (void *)((uint64_t) block + sizeof(m_header));
	}
	else if(block == NULL) {
		uint64_t temp_block = ret_brk();
		uint64_t end_block = (uint64_t)temp_block + alloc_size;
		int status = brk((void*) end_block);
		if(status == -1) {
		//	puts("Not Possible to allocate More memory");
		}
		else {
			m_header *new_header = (m_header*)temp_block;
			new_header->size = alloc_size;
			new_header->available = 0;
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
void free(void *addr) {
	m_header *free_block = (m_header*)((uint64_t)addr - sizeof(m_header));
	free_block->available = 1;
	memset((uint8_t*) ((uint64_t) addr), 0, free_block->size - sizeof(m_header));
}
