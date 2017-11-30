#include <sys/elf64.h>
#include <sys/initfs.h>
#include <sys/tarfs.h>
#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/kernel_threads.h>
#include <sys/paging.h>
#include <sys/kmemcpy.h>
#include <sys/kstring.h>
#include <sys/utils.h>
#include <sys/kmalloc.h>

inode *make_inode(uint64_t size, uint64_t start, int perm) {
	inode *ino = (inode*) kmalloc(sizeof(inode));
	ino->i_size = size;
	ino->i_start = start;
	ino->i_perm = perm;
	return ino;
}

void make_dentry(dentry *parent, dentry *child, char* name, uint64_t begin, uint64_t end, int type, inode *in) {
	kstrcpy(child->d_name, name);
        child->d_ino = in;
        child->d_begin = begin;
        child->d_end = end;
        child->d_children[0] = parent;
        child->d_children[1] = child;
        child->d_type = type;

}

void populate_dentry(char *name, int type, uint64_t start, uint64_t end) {
	
}
void parse_tarfs() {
        struct posix_header_ustar * header = (struct posix_header_ustar *)&_binary_tarfs_start;
        while(header<(struct posix_header_ustar *)&_binary_tarfs_end) {
                uint64_t size = octalToDecimal(stoi(header->size));
		if(header->name[0] == 0) break;
		if (kstrcmp(header->typeflag, "5") == 0) {
			populate_dentry(header->name, DIRECTORY, 0, 2); 
		} else {
			populate_dentry(header->name, FILE, (uint64_t)(header + 1), (uint64_t)((void *)header + 512 + size));
		}
                if (size == 0) {
                        //kprintf("\nFileName: %s\n", header->name);
			header++;
                }
                else {
                        //kprintf("\nFileName: %s\n", header->name);
			size = (size%512==0) ? size +512: size + 512 + (512-size%512);
			header = (struct posix_header_ustar *) (((uint64_t)(header)) + size);
		}
	}
}

void initfs() {
	root_node = (dentry *) kmalloc (sizeof(dentry));
	make_dentry(root_node, root_node, "/", 0, 2, DIRECTORY, make_inode(0, 0, O_RDWR));
	dentry *temp_node = (dentry*) kmalloc(sizeof(dentry));
	make_dentry(root_node, temp_node, "rootfs", 0, 2, DIRECTORY, make_inode(0, 0, O_RDWR));
	root_node->d_children[2] = temp_node;
	parse_tarfs();
} 
