#ifndef NKERNEL_DISK
#define NKERNEL_DISK
// --------------------------------------------------
// This is not currently used, but it's in the works.
// --------------------------------------------------

#include "NLIBC/stdint.h"
#include "NLIBC/io.h"

// For ATA/IDE support.
#define ATA_PRIMARY_DATA 0x1F0
#define ATA_PRIMARY_SECTOR_COUNT 0x1F2
#define ATA_PRIMARY_LBA_LOW 0x1F3
#define ATA_PRIMARY_LBA_MID 0x1F4
#define ATA_PRIMARY_LBA_HIGH 0x1F5
#define ATA_PRIMARY_DEVICE 0x1F6
#define ATA_PRIMARY_COMMAND 0x1F7
#define ATA_PRIMARY_STATUS 0x1F7

void ATA_read_sector(uint32_t lba, uint8_t* buff){
  while(inb(ATA_PRIMARY_STATUS) & 0x80); // Wait until drive is busy.
}

#endif
