#include <sys/kernel_threads.h>
#include <sys/initfs.h>
#include <sys/kmalloc.h>
#include <sys/kprintf.h>
int allocate_file_object(dentry *file_entry) {
	if(file_entry->d_type == DIRECTORY) {
		return -1;
	}
	int i;
	for(i  = 3; i < MAX_FDS; i++ ) {
		if(CURRENT_TASK->file_desc[i] == NULL) 
			break;
	}
	if(i == MAX_FDS) return -1;
	CURRENT_TASK->file_desc[i] = (struct FILE_OBJ*) kmalloc(sizeof(struct FILE_OBJ));
	CURRENT_TASK->file_desc[i]->file_offset = 0;
	CURRENT_TASK->file_desc[i]->file_begin = file_entry->d_begin;
	CURRENT_TASK->file_desc[i]->file_end = file_entry->d_end;
	CURRENT_TASK->file_desc[i]->file_ref_count = 1;
	return i;
}

void deallocate_file_object(int fd) {
	if(CURRENT_TASK->file_desc[fd] != NULL) {
		if(CURRENT_TASK->file_desc[fd]->file_ref_count == 1) {
			kfree((uint64_t*)CURRENT_TASK->file_desc[fd]);
			//kprintf("\nDeleted");
		}
		else
			CURRENT_TASK->file_desc[fd]->file_ref_count--;
	}
} 
