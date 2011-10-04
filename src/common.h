#pragma once
#include "config.h"
#include "types.h"
#include "memorymanager.h"

#if ARCH == ARCH_X86_32
inline void* operator new (unsigned int size)
#elif ARCH == ARCH_X86_64
inline void* operator new (long unsigned int size)
#endif
{ return memory_manager::malloc(size); }

inline void operator delete (void* address)
{ return memory_manager::free(address); }

#if ARCH == ARCH_X86_32
inline void* operator new [] (unsigned int size)
#elif ARCH == ARCH_X86_64
inline void* operator new [] (long unsigned int size)
#endif
{ return memory_manager::malloc(size); }

inline void operator delete [] (void* address)
{ return memory_manager::free(address); }
