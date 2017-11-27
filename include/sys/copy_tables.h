#ifndef _COPY_TABLES_H
#define _COPY_TABLES_H

uint64_t create_table();
uint64_t copy_on_write();
void delete_page_tables(uint64_t cr3);

#endif
