#include "diskmbr.h"
#include <string.h>
#include <stdio.h>

void PrintHelp();

int main(int argc, char** argv)
{
  if (argc <= 1)
  {
    PrintHelp();
    return 0;
  }

  if (!strcmp(argv[1], "-c"))
    WriteMBRCode(stdin, stdout);
  else if (!strcmp(argv[1], "-pt"))
    WriteMBRPartitionTable(stdin, stdout);
  else if (!strcmp(argv[1], "-ds"))
    WriteMBRDiskSignature(stdin, stdout);
  else if (!strcmp(argv[1], "-create"))
  {
    FILE* codeFile = fopen(argv[2], "rb");
    CreateMBR(codeFile, stdout, 0xDDDD);
  }
  
  return 0;
}

void PrintHelp()
{
  printf("diskmbr [-pc] [-pds] [-pp] [-c code_file [disk_signature]]\n"
         "-create code_file disk_signature Create new MBR to stdout. This will leave the partition table null.\n"
         "-c Write code from MBR in stdin to stdout.\n"
         "-ds Write data signature from MBR in stdin to stdout.\n"
         "-pt Write the primary partitions table from MBR in stdin to stdout.\n");
}
