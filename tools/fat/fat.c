#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>

typedef struct 
{
    uint8_t BootJumpInstruction[3];
    uint8_t OemIdentifier[8];
    uint16_t BytesPerSector;
    uint8_t SectorsPerCluster;
    uint16_t ReservedSectors;
    uint8_t FatCount;
    uint16_t DirEntryCount;
    uint16_t TotalSectors;
    uint8_t MediaDescriptorType;
    uint16_t SectorsPerFat;
    uint16_t SectorsPerTrack;
    uint16_t Heads;
    uint32_t HiddenSectors;
    uint32_t LargeSectorCount;

    // extended boot record
    uint8_t DriveNumber;
    uint8_t _Reserved;
    uint8_t Signature;
    uint32_t VolumeId;          // serial number, value doesn't matter
    uint8_t VolumeLabel[11];    // 11 bytes, padded with spaces
    uint8_t SystemId[8];

} __attribute__((packed)) BootSector; // removes the padding.

typedef struct {
  uint8_t Name[11];
  uint8_t Attributes;
  uint8_t _Reserved;
  uint8_t CreateTimeTenths;
  uint16_t CreatedTime;
  uint16_t CreatedDate;
  uint16_t AccessedDate;
  uint16_t FirstClusterHigh;
  uint16_t ModifiedTime;
  uint16_t ModifiedDate;
  uint16_t FirstClusterLow;
  uint32_t Size;
} __attribute__((packed)) DirectoryEntry;

BootSector g_BootSector;
uint8_t* g_fat = NULL;
DirectoryEntry* g_RootDirectory = NULL;
uint32_t g_RootDirectoryEnd;

bool readBootSector(FILE* disk){
  return fread(&g_BootSector, sizeof(g_BootSector), 1, disk) > 0; // Read the boot sector and store it into the Disk file pointer.
}

// Of course, reads the sectors.
bool readSectors(FILE* disk, uint32_t lba, uint32_t count, void* outputBuffer){
  bool ok = true;
  ok = ok && (fseek(disk, lba * g_BootSector.BytesPerSector, SEEK_SET) == 0); // Seeks to the sector we want to read.
  ok = ok && (fread(outputBuffer, g_BootSector.BytesPerSector, count, disk) == count); // Actually reads those sectors, fread() returns the amount of elements read, so we check if they are equal.
  return ok;
}

// Reads and loads the File allocation table onto memory.
bool readFat(FILE* disk){ 
  g_fat = (uint8_t*) malloc(g_BootSector.BytesPerSector * g_BootSector.SectorsPerFat); // Allocate enough memory for the FAT's
  return readSectors(disk, g_BootSector.ReservedSectors, g_BootSector.SectorsPerFat, g_fat); // read from LBA 1 (LBA 0 is the reserved sector, and FAT is at LBA 1) and save it into g_fat pointer.
}

// Reads and loads the Root Directory onto memory
bool readRootDirectory(FILE* disk){
  uint32_t lba = g_BootSector.ReservedSectors + g_BootSector.SectorsPerFat * g_BootSector.FatCount; // Calculate the location of the Root directory in lba.
  uint32_t size = sizeof(DirectoryEntry) * g_BootSector.DirEntryCount; // Calc. the size of Root Dir in bytes.
  uint32_t sectors = (size / g_BootSector.BytesPerSector); // Convert the size from bytes to sectors.

  // If the sectors have any decimal values(for eg -> 4.09, it gets floored by uint32_t.)
  if(size % g_BootSector.BytesPerSector > 0) sectors++; // Add 1 to the sector if it happens.

  g_RootDirectoryEnd = lba + sectors; // Save the directory end to minimize calculations.
  g_RootDirectory = (DirectoryEntry*) malloc(sectors * g_BootSector.BytesPerSector);
  return readSectors(disk, lba, sectors, g_RootDirectory);
}

// Returns the pointer to the metadata of the file.
DirectoryEntry* findFile(const char* name){
  // Loop through all the Entries currently loaded in memory
  for(uint32_t i = 0; i < g_BootSector.DirEntryCount; i++){
    // Find the file.
    if(memcmp(name, g_RootDirectory[i].Name, 11) == 0){
      return &g_RootDirectory[i]; // Return it's location in memory if it exists.
    }
  }

  return NULL; // else return NULL.
}

// Reads a file.
bool readFile(DirectoryEntry* fileEntry, FILE* disk, uint8_t* outputBuffer){

  bool ok = true;
  uint16_t currentCluster = fileEntry->FirstClusterLow; // First cluster low is the cluster the file is in.

  do {
    uint32_t lba = g_RootDirectoryEnd + (currentCluster - 2) * g_BootSector.SectorsPerCluster; // Current LBA we are reading.
    ok = ok && readSectors(disk, lba, g_BootSector.SectorsPerCluster, outputBuffer); // Reads a cluster at once.
    outputBuffer += g_BootSector.SectorsPerCluster * g_BootSector.BytesPerSector; // Shifts the pointer to the output Buffer to the end of the cluster we just read. 

    uint32_t fatIndex = currentCluster * 3 / 2;

    if(currentCluster % 2 == 0){
      currentCluster = (*(uint16_t*)(g_fat + fatIndex)) & 0x0FFF; // Only need the bottom 12 bits, so applying bitmask to remove the bits on the top.
    } else{
      currentCluster = (*(uint16_t*)(g_fat + fatIndex)) >> 4;
    }
  } while(ok && currentCluster < 0xFF8); // 0xFF8 marks the EOF,
  
  return ok;
}

int main(int argc, char* argv[]){

  if(argc < 3){
    printf("Usage: %s [disk Image] [file name]\n", argv[0]);
    return -1;
  }

  FILE* disk = fopen(argv[1], "rb");

  if(!disk){
    fprintf(stderr, "Cannot read the disk!\n");
    return -1;
  }

  if(!readBootSector(disk)){
    fprintf(stderr, "Could not read the boot sector!\n");
    return -2;
  }

  if(!readFat(disk)){
    fprintf(stderr, "Could not load FAT onto memory!\n");
    free(g_fat);
    return -3;
  }

  if(!readRootDirectory(disk)){
    fprintf(stderr, "Could not read the root directory!\n");
    free(g_fat);
    free(g_RootDirectory);
    return -4;
  }

  DirectoryEntry* fileEntry = findFile(argv[2]);
  if(!fileEntry){
    fprintf(stderr, "Could not find the file named %s\n", argv[2]);
    free(g_fat);
    free(g_RootDirectory);
    return -5;
  }
  
  uint8_t* buff = (uint8_t*) malloc(fileEntry->Size + g_BootSector.BytesPerSector); // Alloc 1 extra sector to ensure it works correctly.
  if(!readFile(fileEntry, disk, buff)){
    fprintf(stderr, "Could not read the file named %s\n", argv[2]);
    free(g_fat);
    free(g_RootDirectory);
    free(buff);
    return -6;
  }

  for(size_t i = 0; i < fileEntry->Size; i++){
    printf("%c", buff[i]);
  }
  printf("\n");

  free(buff);
  free(g_fat); // Remember to free the allocation of g_fat.
  free(g_RootDirectory); // Remember to free the allocation of g_RootDirectory.
  return 0;
}
