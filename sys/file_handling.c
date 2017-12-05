#include <sys/kernel_threads.h>
#include <sys/initfs.h>
#include <sys/kmalloc.h>
#include <sys/kprintf.h>
#include <sys/kstring.h>



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
			CURRENT_TASK->file_desc[fd] = NULL;
		}
		else
			CURRENT_TASK->file_desc[fd]->file_ref_count--;
	}
}
 
int allocate_new_dir(dentry *dir_entry) {
	if(dir_entry->d_type == FILE) {
		return -1;
	}
	int i;
	for(i  = 0; i < MAX_FDS; i++ ) {
		if(CURRENT_TASK->dir_desc[i] == NULL) 
			break;
	}
	if(i == MAX_FDS) return -1;
	CURRENT_TASK->dir_desc[i] = (struct DIRK*) kmalloc(sizeof(DIRK));
	DIRK* open_dir = CURRENT_TASK->dir_desc[i];
	kstrcpy(open_dir->d_name, dir_entry->d_name);
	open_dir->d_current = 1;
	open_dir->d_entry = dir_entry;
	open_dir->dir_ref_count = 1;
	return i;
}

void deallocate_new_dir(int fd) {
	if(CURRENT_TASK->dir_desc[fd] != NULL) {
		if(CURRENT_TASK->dir_desc[fd]->dir_ref_count == 1) {
			kfree((uint64_t*)CURRENT_TASK->dir_desc[fd]);
			CURRENT_TASK->dir_desc[fd] = NULL;
		}
		else
			CURRENT_TASK->dir_desc[fd]->dir_ref_count--;
	}
} 
