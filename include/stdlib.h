#ifndef _STDLIB_H
#define _STDLIB_H
#include <sys/defs.h>
int main(int argc, char *argv[], char *envp[]);
void exit(int status);

typedef struct block_header {
	uint8_t available;
	uint64_t size;
	struct block_header *next;
} m_header;

void *malloc(size_t size);
void free(void *ptr);
void setenv(char *name, char *value, char *envp[]);



#endif
