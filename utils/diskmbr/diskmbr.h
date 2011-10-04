#pragma once
#include <stdio.h>
#include <stdint.h>

// Writes MBR code to out
void WriteMBRCode(FILE* mbr, FILE* out);

// Writes MBR partition table to out
void WriteMBRPartitionTable(FILE* mbr, FILE* out);

// Writes MBR data signature to out
void WriteMBRDiskSignature(FILE* mbr, FILE* out);

// Writes new MBR data to out
void CreateMBR(FILE* code, FILE* out, uint32_t diskSignature);
