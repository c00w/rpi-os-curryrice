#include "pit.h"
#include "x86.h"

namespace x86
{
  namespace pit
  {
    void init()
    {
      OUTB(0x43, 0x36);
      OUTB(0x40, 0);
      OUTB(0x40, 0);
    }
  }
}
