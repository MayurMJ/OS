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
	static m_header start = NULL;
	static m_header end = NULL;
	size_t alloc_size = (size + sizeof(m_header) + 15 ) & ~(15);
	alloc_size = alloc_size << 1;
}
