#include "kernel.h"

namespace kernel
{
  static void init();
}

extern unsigned long start_ctors, end_ctors, start_dtors, end_dtors;

extern "C" void kernel_entry()
{
  // Call static constructors
  for (unsigned long *constructor(&start_ctors); constructor < &end_ctors; ++constructor)
    ((void (*) (void)) (*constructor)) ();

  kernel::init();
  kernel::main();

  // Call static destructors
  for (unsigned long *destructor(&start_dtors); destructor < &end_dtors; ++destructor)
    ((void (*) (void)) (*destructor)) ();
}

namespace kernel
{
  static void init()
  {
    x86::pic::init();
    x86::pit::init();
    keyboard::init();
    x86::idt::init();
    text_graphics::init();
    memory_manager::init();
  }
}

extern "C" void __cxa_pure_virtual()
{}

void *__dso_handle;
 
extern "C" int __cxa_atexit(void (*destructor) (void *), void *arg, void *dso)
{ return 0; }

extern "C" void __cxa_finalize(void *f)
{}
