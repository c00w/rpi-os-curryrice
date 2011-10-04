#pragma once
#include "common.h"

namespace x86
{
  //
  // Global Descriptor Table (GDT)
  //
  namespace gdt
  {
#if PLATFORM == PLATFORM_X86_32
    const uint16_t DEFAULT_CODE_SELECTOR = 8;
#elif PLATFORM == PLATFORM_X86_64
    const uint16_t DEFAULT_CODE_SELECTOR = 16;
#endif
  }
}
