#pragma once
#include "common.h"

namespace x86
{
  namespace pic
  {
    void init();
    void signal_eoi(uint irq);
  }
}
