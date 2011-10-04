#pragma once
#include "types.h"

namespace memory_manager
{
  //! Initializes the memory manager. Called before kernel::main()
  void init();

  //! Allocates memory
  //! \param size The number of bytes of memory to allocate
  //! \return The pointer to the allocated memory. If memory cannot be allocated, NULL.
  void* malloc(uint size);

  //! Deallocates memory
  //! \param addr The pointer to the memory which has been previously allocated by malloc()
  void free(void* addr);
}
