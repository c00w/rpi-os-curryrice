#pragma once
#include "common.h"

namespace x86
{
  //
  // Interrupt Descriptor Table (IDT)
  //
  namespace idt
  {
    // This is an entry in the IDT
    #pragma pack (push, 1)
    struct descriptor_entry
    {
      // Bits 0-16 of interrupt address
      uint16_t baseLow;

      // Code selector in GDT
      uint16_t codeSelector;

      // Reserved (0)
      uint8_t reserved;

      // Bit flags
      uint8_t flags;

#if ARCH == ARCH_X86_64
      // Bits 16-31 of interrupt address
      uint16_t baseMiddle;

      // Bits 32-63 of interrupt address
      uint32_t baseHigh;

      // Reserved (0)
      uint32_t reserved2;
#else
      // Bits 16-31 of interrupt address
      uint16_t baseHigh;
#endif
    };
    #pragma pack (pop)

    //! Must be in the format 0D110, where D is descriptor type
    enum DESCRIPTOR_FLAG
    {
      DESCRIPTOR_FLAG_BIT16   = 0x06, //00000110
      DESCRIPTOR_FLAG_BIT32   = 0x0E, //00001110
      DESCRIPTOR_FLAG_RING1   = 0x40, //01000000
      DESCRIPTOR_FLAG_RING2   = 0x20, //00100000
      DESCRIPTOR_FLAG_RING3   = 0x60, //01100000
      DESCRIPTOR_FLAG_PRESENT = 0x80 //10000000
    };

    typedef void (/*_cdecl*/ *interrupt_proc)(void);

    //! Initialize IDT with basic routines
    void init();

    //! Get an interrupt from the IDT
    //! \param index The interrupt index
    interrupt_proc* interrupt(uint8_t index);

    //! Install interrupt routine
    void install_interrupt(interrupt_proc, uint8_t index, uint8_t flags, uint16_t codeSelector);

    volatile descriptor_entry* descriptor(uint8_t index);

    uint idt_base();
    uint interrupt_count();
  }
}
