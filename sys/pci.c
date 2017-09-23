#include <sys/defs.h>
#include <sys/kprintf.h>
static int count = 0;
static inline uint32_t SysInLong(unsigned short readAddress) {
   uint32_t tmp2;
    __asm__ __volatile("inl %1,%0"
                       :"=a"(tmp2)
                       :"Nd"(readAddress));
   return tmp2;

}
uint16_t pciConfigReadWord (uint8_t bus, uint8_t slot,
                             uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;
    //uint32_t tmp2 = 0;
 
    /* create configuration address as per Figure 1 */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
 
    /* write out the address */
    //sysOutLong (0xCF8, address);
    unsigned short writeAddress = 0xcf8;
    __asm__ __volatile("outl %0,%1"
		       :
                       :"a"(address), "Nd"(writeAddress));

    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
    //unsigned short readAddress = 0xcfc;
    //__asm__ __volatile("inl %1,%0"
      //                 :"=a"(tmp2)
	//	       :"Nd"(readAddress));
    tmp = (uint16_t)(((SysInLong(0xCFC)) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
}

uint16_t pciCheckVendor(uint8_t bus, uint8_t slot) {
    uint16_t vendor, device;
    /* try and read the first configuration register. Since there are no */
    /* vendors that == 0xFFFF, it must be a non-existent device. */
    if ((vendor = pciConfigReadWord(bus,slot,0,0)) != 0xFFFF) {
       device = pciConfigReadWord(bus,slot,0,8);
       if(device == 0x106)
       count++;
    }
 //kprintf("wtf");
 return (vendor);
}
void enumerate_pci() {
uint8_t bus;
int8_t device;
 
     for(bus = 0; bus < 256; bus++) {
         for(device = 0; device < 32; device++) {
             pciCheckVendor(bus, device);
             kprintf("\n Checking Device %d, %d", bus, device);
         }
        if(bus == 255 ) break;
     }
 kprintf("\nDevvices found %d", count);
}


