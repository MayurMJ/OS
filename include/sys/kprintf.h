#ifndef __KPRINTF_H
#define __KPRINTF_H

void shift_up();
void display();
int put_char_into_buffer(char c);
void kprintf(const char *fmt, ...);
void init_buffer();
int put_stdin_into_buffer(char c);
void local_echo();
#endif
