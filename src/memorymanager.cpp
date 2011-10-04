#include "memorymanager.h"

namespace memory_manager
{
  enum REGION_TYPE
  {
    REGION_TYPE_UNKNOWN,
    REGION_TYPE_USABLE,
    REGION_TYPE_RESERVED,
    REGION_TYPE_ACPI_RECLAIMABLE,
    REGION_TYPE_ACPI_NVS,
    REGION_TYPE_BAD
  };

  #pragma pack(push, 1)
  struct memory_region
  {
    uint32_t base_addr_low;
    uint32_t base_addr_high;
    uint32_t len_low;
    uint32_t len_high;
    uint32_t type;
    uint32_t extended_attr;
  };
  #pragma pack (pop)

  static inline const memory_region* regions()
  { return (memory_manager::memory_region*)0xD00; }

  const memory_region* region(uint index)
  { return regions() + index; }

  uint region_count()
  { return static_cast<uint>(*((uint32_t*)0x7bfb)); }

  void init()
  {
    // Write your code here!
  }

  void* malloc(uint size)
  {
    // Write your code here!
    return 0;
  }

  void free(void* memory)
  {
    // Write your code here!
  }
};
