#ifndef _SCHEDULER_H
#define _SCHEDULER_H
#include <sys/defs.h>
#include <sys/kernel_threads.h>
#include <sys/elf64.h>
#include <sys/paging.h>
void put_in_run_queue(Task *newtask);
#endif
