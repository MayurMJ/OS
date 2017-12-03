#include <sys/kernel_threads.h>
#include <sys/initfs.h>
#include <sys/kmalloc.h>
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
	return i;
} 
