#pragma once
#include "common.h"
#include "key.h"

namespace keyboard
{
  void init();

  KEY get_key();
  uint get_char();
}
