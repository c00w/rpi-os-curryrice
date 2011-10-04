#include "diskmbr.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static const unsigned int MBR_CODE_SIZE = 440, MBR_PARTITION_TABLE_OFFSET = 446, MBR_PARTITION_TABLE_SIZE = 64, MBR_DISK_SIGNATURE_OFFSET = 440, MBR_DISK_SIGNATURE_SIZE = 4, MBR_SIZE = 512;

static const uint16_t mbrSignature  = 0xAA55;

#define PIPE(IN, OUT, SIZE)\
  do\
  {\
    uint8_t buff[SIZE];\
    size_t count = fread(buff, (SIZE), 1, (IN));\
    fwrite(buff, (SIZE), 1, (OUT));\
    fflush((OUT));\
  } while (0);

void WriteMBRCode(FILE* mbr, FILE* out)
{
  PIPE(mbr, out, MBR_CODE_SIZE);
}

void WriteMBRPartitionTable(FILE* mbr, FILE* out)
{
  fseek(mbr, MBR_PARTITION_TABLE_OFFSET, SEEK_CUR);
  PIPE(mbr, out, MBR_PARTITION_TABLE_SIZE);
}

void WriteMBRDiskSignature(FILE* mbr, FILE* out)
{
  fseek(mbr, MBR_DISK_SIGNATURE_OFFSET, SEEK_CUR);
  PIPE(mbr, out, MBR_DISK_SIGNATURE_SIZE);
}

void CreateMBR(FILE* code, FILE* out, uint32_t diskSignature)
{
  uint8_t null[MBR_PARTITION_TABLE_SIZE];
  memset(null, 0, MBR_PARTITION_TABLE_SIZE);
  PIPE(code, out, MBR_CODE_SIZE);
  fwrite(&diskSignature, MBR_DISK_SIGNATURE_SIZE, 1, out);
  fwrite(null, 2, 1, out);
  fwrite(null, MBR_PARTITION_TABLE_SIZE, 1, out);
  fwrite(&mbrSignature, 2, 1, out);
}
