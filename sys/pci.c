#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/ahci.h>
static inline uint32_t SysInLong(unsigned short readAddress) {
   uint32_t tmp2;
    __asm__ __volatile("inl %1,%0"
                       :"=a"(tmp2)
                       :"Nd"(readAddress));
   return tmp2;

}
static inline void SysOutLong(unsigned short writeAddress, uint32_t address) {
    __asm__ __volatile("outl %0,%1"
                       :
                       :"a"(address), "Nd"(writeAddress));
}
uint32_t tmpReadWord (uint8_t bus, uint8_t slot,
                             uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func; 
    uint32_t tmp2 = 0;
    /* create configuration address as per Figure 1 */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
                       
    unsigned short writeAddress = 0xcf8;
    __asm__ __volatile("outl %0,%1"
                       :
                       :"a"(address), "Nd"(writeAddress));
        
    tmp2=SysInLong(0xcfc); 
    return (tmp2);
}

uint16_t pciConfigReadWord (uint8_t bus, uint8_t slot,
                             uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;
    /* create configuration address as per Figure 1 */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
 
    unsigned short writeAddress = 0xcf8;
    __asm__ __volatile("outl %0,%1"
		       :
                       :"a"(address), "Nd"(writeAddress));

    tmp = (uint16_t)(((SysInLong(0xCFC)) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
}

uint16_t pciCheckVendor(uint8_t bus, uint8_t slot) {
    uint16_t vendor;
    uint32_t device;
    uint32_t bar5;
    uint32_t tempbar;
    uint32_t bar5location;
    /* try and read the first configuration register. Since there are no */
    /* vendors that == 0xFFFF, it must be a non-existent device. */
    if ((vendor = pciConfigReadWord(bus,slot,0,0)) != 0xFFFF) {
       device = tmpReadWord(bus,slot,0,8);
       if((device >> 16) == 0x106) {
	bar5 = tmpReadWord(bus, slot, 0, 0x24);
	bar5location = (uint32_t)((bus << 16) | (slot << 11) |
              	       (0 << 8) | (0x24 & 0xfc) | ((uint32_t)0x80000000));
	SysOutLong(0xcf8,bar5location);
	SysOutLong(0xcfc, 0xffffffff);
	hba_mem_t hbamemstruct = *((hba_mem_t *)0xffffffff);
	tempbar = tmpReadWord(bus, slot, 0, 0x24);
        kprintf("%x\n", tempbar);
	kprintf("%x\n",hbamemstruct.pi);
	SysOutLong(0xcf8,bar5location);
        SysOutLong(0xcfc, bar5);
	return 1;
       }
    }
    return 0;
}
void enumerate_pci() {
     uint8_t bus;
     uint8_t device;
     int status=0;
 
     for(bus = 0; bus < 256; bus++) {
         for(device = 0; device < 32; device++) {
             status = pciCheckVendor(bus, device);
	     if (status == 1)
		break;
         }
	 if (status == 1) break;
     }
}


