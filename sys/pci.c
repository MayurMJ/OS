#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/pci.h>
#include <sys/ahci.h>
#include <sys/kmemcpy.h>
#define TRUE 1
#define FALSE 0
void port_rebase(hba_port_t *port, int portno, hba_mem_t *abar);
void stop_cmd(hba_port_t *port);
void stop_tmp_cmd(hba_port_t *port);
void start_tmp_cmd(hba_port_t *port);
void start_cmd(hba_port_t *port);
static inline uint32_t SysInLong(unsigned short readAddress) {
   uint32_t tmp2;
    __asm__ __volatile("inl %1,%0"
                       :"=a"(tmp2)
                       :"Nd"(readAddress));
   return tmp2;

}
static inline void SysOutLong(uint16_t writeAddress, uint32_t address) {
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

// Check device type
static int check_type(hba_port_t *port)
{
        uint32_t ssts = port->ssts;

        uint8_t ipm = (ssts >> 8) & 0x0F;
        uint8_t det = ssts & 0x0F;

        if (det == HBA_PORT_DET_PRESENT)        // Check drive status
                return AHCI_DEV_NULL;
        if (ipm == HBA_PORT_IPM_ACTIVE)
                return AHCI_DEV_NULL;

        switch (port->sig)
        {
        case SATA_SIG_ATAPI:
                return AHCI_DEV_SATAPI;
        case SATA_SIG_SEMB:
                return AHCI_DEV_SEMB;
        case SATA_SIG_PM:
                return AHCI_DEV_PM;
        default:
                return AHCI_DEV_SATA;
        }
}
hba_port_t* probe_port(hba_mem_t *abar)
{
	// Search disk in impelemented ports
	abar->ghc = (1 << 31);
	//abar->ghc = (1 << 0);
	//abar->ghc = (1 << 31);
	for (int x=0;x< 10000000;x++);
	for (int x=0;x< 10000000;x++);
	uint32_t pi = abar->pi;
	//* kprintf("\nPi inside:%x", abar->pi);
	int i = 0;
	while (i<32)
	{
		if (pi & 1)
		{
			int dt = check_type(&abar->ports[i]);
			if (dt == AHCI_DEV_SATA)
			{
				port_rebase(&abar->ports[i], i, abar);
				//if(i != 0) {
				kprintf("SATA drive found at port %d\n", i);
				uint64_t buf = (uint64_t)0x3ff9c000;// + 1024 + 256 + 928 *32;

  				memset((void *)buf,0,409600);

 				uint64_t tmpbuf = buf;
  				int j;

  //kprintf("\nbefore kmemset %d",*((uint8_t *)(buf)));
  				for(j=0;j < 100;j++) {
        				memset((uint8_t *)tmpbuf,j,4096);
        				tmpbuf += 4096;
  				}

  				int sectorIndex = 0;
  				for(sectorIndex = 0; sectorIndex < 100; sectorIndex++) {
        				for(int j = 0; j < 1000000; j++);
        				for(int j = 0; j < 1000000; j++);
					//port_rebase(&abar->ports[i], i, abar);
					if (sectorIndex != 0) {
					stop_tmp_cmd(&abar->ports[i]);
					start_tmp_cmd(&abar->ports[i]);
					}
					//* kprintf("write number %d\n",sectorIndex);
        				write(&abar->ports[i], sectorIndex*8, 0, 8, buf + (sectorIndex *8 * 512));
				//	kprintf("\nOut Of write");
				}
				tmpbuf = buf;
  				for(j=0;j < 100;j++) {
        				memset((uint8_t *)tmpbuf,0,4096);
        				tmpbuf += 4096;
  				}
  				for(sectorIndex = 0; sectorIndex < 100; sectorIndex++) {
        				for(int j = 0; j < 1000000; j++);
        				for(int j = 0; j < 1000000; j++);
					//port_rebase(&abar->ports[i], i, abar);
					if (sectorIndex != 0) {
					stop_tmp_cmd(&abar->ports[i]);
					start_tmp_cmd(&abar->ports[i]);
					}
					//* kprintf("read number %d\n",sectorIndex);
        				read(&abar->ports[i], sectorIndex*8, 0, 8, buf + (sectorIndex *8 * 512));
					kprintf("buf value %d %d\n",*((uint8_t *)(buf + (sectorIndex *8 * 512))),
								 *((uint8_t *)(buf + (sectorIndex *8 * 512) + 4095)));
				}



				///*
				//port_rebase(&abar->ports[i], i, abar);
				return &abar->ports[i];
				//}
			}
			else if (dt == AHCI_DEV_SATAPI)
			{
				kprintf("SATAPI drive found at port %d\n", i);
			}
			else if (dt == AHCI_DEV_SEMB)
			{
				kprintf("SEMB drive found at port %d\n", i);
			}
			else if (dt == AHCI_DEV_PM)
			{
				kprintf("PM drive found at port %d\n", i);
			}
			else
			{
				//kprintf("No drive found at port %d\n", i);
			}
		}
 
		pi >>= 1;
		i ++;
	}
	return NULL;
}
 

uint16_t pciCheckVendor(uint8_t bus, uint8_t slot, uint8_t function) {
    uint16_t vendor;
    uint32_t device;
//    vendor = pciConfigReadWord(bus,slot,0,0);
    
   // for(function = 0;function < 8; function++) {  
	    /* try and read the first configuration register. Since there are no */
	    /* vendors that == 0xFFFF, it must be a non-existent device. */
	    if ((vendor = pciConfigReadWord(bus,slot,function,0)) != 0xFFFF) {
	       device = tmpReadWord(bus,slot,function,8);
	       if((device >> 16) == 0x106) {
		//* kprintf("AHCI controller found %d %d %d\n",bus,slot,function);
		return 1;
	       }
	    }
    // }
    return 0;
}
hba_port_t* enumerate_pci() {
     uint8_t bus;
     uint8_t device;
     int status=0;
     uint8_t function;
     uint32_t bar5location = 0;
     for(bus = 0; bus < 256; bus++) {
         for(device = 0; device < 32; device++) {
	     for(function =0; function<8; function++) {
             	     status = pciCheckVendor(bus, device, function);
		     if (status == 1) {
			kprintf("\nAHCI controller found %d %d\n",bus,device);
			bar5location = (uint32_t)((bus << 16) | (device << 11) |
			       (function << 8) | (0x24 & 0xfc) | ((uint32_t)0x80000000));
			SysOutLong(0xcf8,bar5location);
			SysOutLong(0xcfc, 0xa6000);
			hba_mem_t *hbamemstruct = (hba_mem_t *)0xa6000;
			//kprintf("PI %x\n",hbamemstruct->pi);
			return (probe_port(hbamemstruct));
		     }
	    }
         }
	 if (status == 1) break;
     }
	return NULL;
}

#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08
 
// Find a free command list slot
int find_cmdslot(hba_port_t *port)
{
	// If not set in SACT and CI, the slot is free
	uint32_t slots = (port->sact | port->ci);
	//kprintf("ci value in find_cmdslot %x\n",port->ci);
	for (int i=0; i<32; i++)
	{
		if ((slots&1) == 0)
			return i;
		slots >>= 1;
	}
	// kprintf("Cannot find free command list entry\n");
	return -1;
}

#define	AHCI_BASE	0x400000
 
// Start command engine
void start_cmd(hba_port_t *port)
{
	//kprintf("start of start_cmd ssts %x tfd %x\n",port->ssts,port->tfd);
	port->sctl |= 0x10;
	port->sctl |= 0x301;
	int x;
        for (x=0;x< 10000000;x++);
        for (x=0;x< 10000000;x++);
        for (x=0;x< 10000000;x++);
	port->sctl = 0x300;
	while((port->ssts & 0x3) != 0x3);
	port->cmd = 0x10000016;
        for (x=0;x< 10000000;x++);
        for (x=0;x< 10000000;x++);
	port->serr_rwc = 0xffffffff;

	while (port->cmd & HBA_PxCMD_CR);
	port->cmd |= HBA_PxCMD_FRE;
	port->cmd |= HBA_PxCMD_ST;

	//kprintf("end of start_cmd ssts %x tfd %x\n",port->ssts,port->tfd);
}



void start_tmp_cmd(hba_port_t *port)
{
        //kprintf("start of start_tmp_cmd ssts %x tfd %x\n",port->ssts,port->tfd);
     /*   port->sctl |= 0x10;
        port->sctl |= 0x301;
        int x;
        for (x=0;x< 10000000;x++);
        for (x=0;x< 10000000;x++);
        for (x=0;x< 10000000;x++);
        port->sctl = 0x300;
        while((port->ssts & 0x3) != 0x3);
        port->cmd = 0x10000016;
        for (x=0;x< 10000000;x++);
        for (x=0;x< 10000000;x++);
        port->serr_rwc = 0xffffffff;

*/
	//port->cmd = 0x10000016;
        port->cmd |= HBA_PxCMD_FRE;
	port->cmd |= 0x2;
	port->cmd |= 0x8;
        int x;
	for (x=0;x< 10000000;x++);
        for (x=0;x< 10000000;x++);
	while(1) {
		if((((port->ssts & 0xf)==0x1) || ((port->ssts & 0xf)==0x3)) && (((port->ssts>>8)&0xf)==0x1))
			break;
	}
	port->serr_rwc = 0xffffffff;
	
        //while (port->cmd & HBA_PxCMD_CR);
        port->cmd |= HBA_PxCMD_ST;

        //kprintf("end of start_tmp_cmd ssts %x tfd %x\n",port->ssts,port->tfd);
}
void stop_tmp_cmd(hba_port_t *port)
{
        // Clear ST (bit0)
        //kprintf("start of stop_tmp_cmd ssts %x tfd %x\n",port->ssts,port->tfd);
        port->cmd &= ~HBA_PxCMD_ST;

        // Wait until FR (bit14), CR (bit15) are cleared
        while(1)
        {
                if (port->cmd & HBA_PxCMD_CR)
                        continue;
                break;
        }
        // Clear FRE (bit4)
        port->cmd &= ~HBA_PxCMD_FRE;
	port->sctl |= 0xf;
        //kprintf("end of stop_tmp_cmd ssts %x tfd %x\n",port->ssts,port->tfd);
} 
// Stop command engine
void stop_cmd(hba_port_t *port)
{
	// Clear ST (bit0)
	//kprintf("start of stop_cmd ssts %x tfd %x\n",port->ssts,port->tfd);
	port->cmd &= ~HBA_PxCMD_ST;
 
	// Wait until FR (bit14), CR (bit15) are cleared
	while(1)
	{
		if (port->cmd & HBA_PxCMD_CR)
			continue;
		break;
	}
	// Clear FRE (bit4)
	while(1)
	{
		if (port->cmd & HBA_PxCMD_FR)
			continue;
		break;
	}
	port->cmd &= ~HBA_PxCMD_FRE;
	//kprintf("end of stop_cmd ssts %x tfd %x\n",port->ssts,port->tfd);
}
void port_rebase(hba_port_t *port, int portno, hba_mem_t *abar)
{
	stop_cmd(port);	// Stop command engine
	/*	port->sctl |= 0x10;
		port->sctl |= 0x301;
		int x;
		for (x=0;x< 10000000;x++);
		for (x=0;x< 10000000;x++);
		for (x=0;x< 10000000;x++);
		port->sctl = 0x300;
		while((port->ssts & 0x3) != 0x3);
		port->cmd = 0x10000016;
		for (x=0;x< 10000000;x++);
		for (x=0;x< 10000000;x++);
		port->serr_rwc = 0xffffffff;
 	*/
	/*port->cmd |= 0x10000000;
	port->cmd |= 0x00000004;
	port->cmd |= 0x00000002;
	port->cmd |= 0x00000008;;
	port->sctl = 0x300;
	for(int i = 0; i < 1000000; i++);
	port->sctl = 0x301;
	port->serr_rwc = 0xffffffff;*/
	// Command list offset: 1K*portno
	// Command list entry size = 32
	// Command list entry maxim count = 32
	// Command list maxim size = 32*32 = 1K per port
	//kprintf("\nchecking ssts  %x tfd %x\n",port->ssts,port->tfd);
	port->clb = AHCI_BASE  + (portno <<10);
	//kprintf("\n BASe:clb %x", AHCI_BASE);
	//port->clbu = 0;
	memset((void*)(port->clb), 0, 1024);
 
	// FIS offset: 32K+256*portno
	// FIS entry size = 256 bytes per port
	port->fb = AHCI_BASE + (32 << 10) + (portno << 8);
	//kprintf("\n BASe:fis %x", AHCI_BASE + 0x400);
	//port->fbu = 0;
	memset((void*)(port->fb), 0, 256);

	// Command table offset: 40K + 8K*portno
	// Command table size = 256*32 = 8K per port
	hba_cmd_header_t *cmdheader = (hba_cmd_header_t*)(port->clb);
	for (int i=0; i<32; i++)
	{
		cmdheader[i].prdtl = 8;	// 8 prdt entries per command table
					// 256 bytes per command table, 64+16+48+16*8
		// Command table offset: 40K + 8K*portno + cmdheader_index*256
		cmdheader[i].ctba = AHCI_BASE + (40 << 10) + (portno << 13) + (i << 8);
		//cmdheader[i].ctbau = 0;
		memset((void*)cmdheader[i].ctba, 0, 256);
	}
	 
	//kprintf("\nbefore start SSTS %x  sctl %x tfd %x",port->ssts,port->sctl,port->tfd);
	start_cmd(port);	// Start command enginei
}
int read(hba_port_t *port, uint32_t startl, uint32_t starth, uint32_t count, uint64_t buf)
{
	port->is_rwc = (uint32_t)-1;		// Clear pending interrupt bits
	int spin = 0; // Spin lock timeout counter
	int slot = find_cmdslot(port);
	if (slot == -1)
		return FALSE;
 
	hba_cmd_header_t *cmdheader = (hba_cmd_header_t*)port->clb;
	cmdheader += slot;
	cmdheader->cfl = sizeof(fis_reg_h2d_t)/sizeof(uint32_t);	// Command FIS size
	cmdheader->w = 0;		// Read from device
	cmdheader->prdtl = (uint16_t)((count-1)>>4) + 1;	// PRDT entries count
 
	hba_cmd_tbl_t *cmdtbl = (hba_cmd_tbl_t*)(cmdheader->ctba);
	memset(cmdtbl, 0, sizeof(hba_cmd_tbl_t) +
 		(cmdheader->prdtl-1)*sizeof(hba_prdt_entry_t));
 
	// 8K bytes (16 sectors) per PRDT
	int i = 0;
	for (i=0; i<cmdheader->prdtl-1; i++)
	{
		cmdtbl->prdt_entry[i].dba = (uint64_t)(buf & 0xFFFFFFFFFFFFFFFF);;
		cmdtbl->prdt_entry[i].dbc = 8*1024;	// 8K bytes
		cmdtbl->prdt_entry[i].i = 1;
		buf += 4*1024;	// 4K words
		count -= 16;	// 16 sectors
	}
	// Last entry
	cmdtbl->prdt_entry[i].dba = (uint64_t)(buf & 0xFFFFFFFFFFFFFFFF);
	cmdtbl->prdt_entry[i].dbc = count<<9;	// 512 bytes per sector
	cmdtbl->prdt_entry[i].i = 1;
 
	// Setup command
	fis_reg_h2d_t *cmdfis = (fis_reg_h2d_t*)(&cmdtbl->cfis);
 
	cmdfis->fis_type = FIS_TYPE_REG_H2D;
	cmdfis->c = 1;	// Command
	cmdfis->command = ATA_CMD_READ_DMA_EX;
 
	cmdfis->lba0 = (uint8_t)startl;
	cmdfis->lba1 = (uint8_t)(startl>>8);
	cmdfis->lba2 = (uint8_t)(startl>>16);
	cmdfis->device = 1<<6;	// LBA mode
 
	cmdfis->lba3 = (uint8_t)(startl>>24);
	cmdfis->lba4 = (uint8_t)starth;
	cmdfis->lba5 = (uint8_t)(starth>>8);
 
	cmdfis->count = count;
	//cmdfis->counth = HIuint8_t(count);
		//kprintf("\%d\n",__LINE__);
 
		//kprintf("tfd =  %d\n",port->tfd);
	// The below loop waits until the port is no longer busy before issuing a new command
	while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
	{
		spin++;
	}
	if (spin == 1000000)
	{
		kprintf("Port is hung\n");
		return FALSE;
	}
 
	port->ci = 1<<slot;	// Issue command

	// Wait for completion
	while (1)
	{
		// In some longer duration reads, it may be helpful to spin on the DPS bit 
		// in the PxIS port field as well (1 << 5)
		if ((port->ci & (1<<slot)) == 0) {
			//kprintf("port ci value %x\n",port->ci);
			break;
		}
		if (port->is_rwc & HBA_PxIS_TFES)	// Task file error
		{
			kprintf("Read disk error\n");
			return FALSE;
		}
	}
 
	// Check again
	if (port->is_rwc & HBA_PxIS_TFES)
	{
		kprintf("Read disk error\n");
		return FALSE;
	}
 
	return TRUE;
} 
int write(hba_port_t *port, uint32_t startl, uint32_t starth, uint32_t count, uint64_t buf)
{
	//kprintf("BEFORE WRITE tfd %x ssts %x sctl %x\n",port->tfd,port->ssts, port->sctl);
//	while(port->tfd != 0x50);
	//kprintf("tfd value after looping in write %x\n",port->tfd);
	port->is_rwc = (uint32_t)-1;		// Clear pending interrupt bits
	int spin = 0; // Spin lock timeout counter
	int slot = find_cmdslot(port);
	//kprintf("slot value is %d\n",slot);
	if (slot == -1)
		return FALSE;
	//port->clb = AHCI_BASE; 
	hba_cmd_header_t *cmdheader = (hba_cmd_header_t*)port->clb;
	cmdheader += slot;
	cmdheader->cfl = sizeof(fis_reg_h2d_t)/sizeof(uint32_t);	// Command FIS size
	cmdheader->w = 1;		// Write to device
//	cmdheader->c = 1;
//	cmdheader->p = 1;
	cmdheader->prdtl = (uint16_t)((count-1)>>4) + 1;	// PRDT entries count
 
	hba_cmd_tbl_t *cmdtbl = (hba_cmd_tbl_t*)(cmdheader->ctba);
	memset(cmdtbl, 0, sizeof(hba_cmd_tbl_t) +
 		(cmdheader->prdtl-1)*sizeof(hba_prdt_entry_t));
 
	// 8K bytes (16 sectors) per PRDT
	int i = 0;
	for (i=0; i<cmdheader->prdtl-1; i++)
	{
		//memset(buf, 1, 4096);
		cmdtbl->prdt_entry[i].dba = (uint64_t)(buf & 0xFFFFFFFFFFFFFFFF) ;//& (0xFFFFFFFF);
		cmdtbl->prdt_entry[i].dbc = 8*1024;	// 8K bytes
		cmdtbl->prdt_entry[i].i = 1;
		buf += 4*1024;	// 4K words
		count -= 16;	// 16 sectors
	}
	// Last entry
	cmdtbl->prdt_entry[i].dba = (uint64_t)(buf & 0xFFFFFFFFFFFFFFFF);//&(0xFFFFFFFF);
	cmdtbl->prdt_entry[i].dbc = count<<9;	// 512 bytes per sector
	cmdtbl->prdt_entry[i].i = 1;
	 
	// Setup command
	fis_reg_h2d_t *cmdfis = (fis_reg_h2d_t*)(&cmdtbl->cfis);
 
	cmdfis->fis_type = FIS_TYPE_REG_H2D;
	cmdfis->c = 1;	// Command
	cmdfis->command = ATA_CMD_WRITE_DMA_EX;
 
	cmdfis->lba0 = (uint8_t)startl;
	cmdfis->lba1 = (uint8_t)(startl>>8);
	cmdfis->lba2 = (uint8_t)(startl>>16);
	cmdfis->device = 1<<6;	// LBA mode
 
	cmdfis->lba3 = (uint8_t)(startl>>24);
	cmdfis->lba4 = (uint8_t)starth;
	cmdfis->lba5 = (uint8_t)(starth>>8);
 
	cmdfis->count = count;
	//cmdfis->counth = HIuint8_t(count);
 
		//kprintf("\%d\n",__LINE__);
	// The below loop waits until the port is no longer busy before issuing a new command
	while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
	{
		spin++;
	}
	if (spin == 1000000)
	{
		kprintf("Port is hung\n");
		return FALSE;
	}
		//kprintf("\%d\n",__LINE__);
 
	port->ci = 1<<slot;	// Issue command
//		kprintf("\%d\n",__LINE__);
	// Wait for completion
	while (1)
	{
		// In some longer duration reads, it may be helpful to spin on the DPS bit 
		// in the PxIS port field as well (1 << 5)
		if ((port->ci & (1<<slot)) == 0) 
			break;
		if (port->is_rwc & HBA_PxIS_TFES)	// Task file error
		{
			kprintf("Read disk error\n");
			return FALSE;
		}
	}
		//kprintf("\%d\n",__LINE__);
// */
	// Check again
	if (port->is_rwc & HBA_PxIS_TFES)
	{
		kprintf("Read disk error\n");
		return FALSE;
	}
	

	int x;
	for (x=0;x<1000000;x++);
	for (x=0;x<1000000;x++);
	for (x=0;x<1000000;x++);
	for (x=0;x<1000000;x++);
	for (x=0;x<1000000;x++);
	for (x=0;x<1000000;x++);
	for (x=0;x<1000000;x++);	 
	return TRUE;
} 
