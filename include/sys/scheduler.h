#ifndef _SCHEDULER_H
#define _SCHEDULER_H
#include <sys/defs.h>
#include <sys/kernel_threads.h>
#include <sys/elf64.h>
#include <sys/paging.h>
void put_in_run_queue(Task *newtask);
void init_scheduler();
void scheduler();
void remove_from_run_queue(Task * removeTask);
void schedule();
uint64_t has_child(Task * parent);
void display_queue();
Task* zombie_child_exists(Task * parent);
void reap_process(Task * reapThis);
void setupTask(Task *task, void (*main)(), Task *otherTask);
#endif
