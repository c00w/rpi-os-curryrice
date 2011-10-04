#pragma once

#define ARCH_X86_32 0
#define ARCH_X86_64 1

#ifndef __ASSEMBLER__
enum CPU_ARCHITECTURE
{
  CPU_ARCHITECTURE_X86_32,
  CPU_ARCHITECTURE_X86_64
};
#endif
