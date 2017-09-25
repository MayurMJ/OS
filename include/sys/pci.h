#ifndef _PCI_H_
#define _PCI_H_
#include <sys/ahci.h>
#include <sys/defs.h>
hba_port_t* enumerate_pci();
int read_port(hba_port_t *port, uint32_t startl, uint32_t starth, uint32_t count, uint16_t *buf);
int write_port(hba_port_t *port, uint32_t startl, uint32_t starth, uint32_t count, uint16_t *buf);

#endif
