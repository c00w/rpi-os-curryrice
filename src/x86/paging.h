#pragma once
#include "apoccommon.h"
#include "apocvirtualmemorysystem.h"

namespace Apoc
{
  namespace X86
  {
    class PAEPagingSystem : VirtualMemorySystem
    {
      void* address;

    public:
      virtual UInt GetPageSize() = 0;
      virtual void SetMapping(void* virtualAddress, void* physicalAddress, UInt pageCount);
    };

    class PAEPaging
  }
}
