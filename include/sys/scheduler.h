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
void reap_all_child(Task *parent);
Task* zombie_child_exists(Task * parent);
void reap_process(Task * reapThis);
void replace_ptr_in_queue(Task * replace, Task * new_task);
void setupTask(Task *task, void (*main)(), Task *otherTask);
int pid_exists(uint64_t pid);
Task * get_task_from_pid(uint64_t pid);
extern uint64_t sleeping_time;
void reparent_orphans(Task *dyingTask);
void display_pid();
#endif
