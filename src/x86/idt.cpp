#include "textgraphics.h"
#include "keyboard.h"
#include "x86/x86.h"
#include "x86/idt.h"
#include "x86/gdt.h"
#include "x86/int.h"

  namespace keyboard
  {
    void irq();
  }

  namespace x86
  {
    namespace idt
    {
      static const uint IDT_BASE = 0x500;
      static const uint NUM_INTERRUPTS = 255;

      #pragma pack(push, 1)
      struct idtr_reg
      {
	// Size of IDT
	uint16_t limit;

	// Base address of IDT
	uint base;
      };
      #pragma pack (pop)

      #define HANDLE_EXCEPTION(REASON) ;

      static void DivideErrorException()
      {
        HANDLE_EXCEPTION("Divide error");
      }

      static void DebugException()
      {
        HANDLE_EXCEPTION("Debug");
      }

      static void NMIinterrupt()
      {
        /* TODO */
      }

      static void BreakpointException()
      {
        HANDLE_EXCEPTION("Breakpoint");
      }

      static void OverflowException()
      {
        HANDLE_EXCEPTION("Overflow");
      }

      static void BoundRangeExceededException()
      {
        HANDLE_EXCEPTION("Bound range exceeded");
      }
      
      static void InvalidOpcodeException()
      {
        HANDLE_EXCEPTION("Invalid opcode");
      }

      static void DeviceNotAvailableException()
      {
        HANDLE_EXCEPTION("Device not available");
      }

      static void DoubleFaultException()
      {
        HANDLE_EXCEPTION("Double fault");
      }

      static void CoprocessorSegmentOverrun()
      {
        HANDLE_EXCEPTION("Coprocessor segment overrun");
      }

      static void InvalidTSSException()
      {
        HANDLE_EXCEPTION("Invalid TSS exception");
      }

      static void SegmentationFaultException()
      {
        HANDLE_EXCEPTION("Segmentation fault");
      }

      static void StackFaultException()
      {
        HANDLE_EXCEPTION("Stack fault");
      }

      static void GeneralProtectionException()
      {
        HANDLE_EXCEPTION("General protection");
      }

      static void PageFaultException()
      {
        HANDLE_EXCEPTION("Page fault");
      }
      
      static void Int15()
      {
        HANDLE_EXCEPTION("Int15");
      }

      static void FPUFloatingPointException()
      {
        HANDLE_EXCEPTION("FPU floating point");
      }

      static void AlignmentCheckException()
      {
        HANDLE_EXCEPTION("Alignment check");
      }

      static void MachineCheckException()
      {
        HANDLE_EXCEPTION("Machine check");
      }

      static void SIMDFloatingPointException()
      {
        HANDLE_EXCEPTION("SIMD floating point");
      }

      void handle_irq(uint irqNumber)
      {
        switch (irqNumber)
	{
	  case 1:
	    keyboard::irq();
	    break;
	}
      }

      #define DINT(NUM) static void Int ## NUM() { ; }
      DINT(20);
      DINT(21);
      DINT(22);
      DINT(23);
      DINT(24);
      DINT(25);
      DINT(26);
      DINT(27);
      DINT(28);
      DINT(29);
      DINT(30);
      DINT(31);

      #define set_builtin_interrupt(ROUTINE, INDEX) install_interrupt((interrupt_proc)(ROUTINE), INDEX, DESCRIPTOR_FLAG_PRESENT | DESCRIPTOR_FLAG_BIT32, gdt::DEFAULT_CODE_SELECTOR);

      void init()
      {
        set_builtin_interrupt(DivideErrorException, 0);
        set_builtin_interrupt(DebugException, 1);
        set_builtin_interrupt(NMIinterrupt, 2);
        set_builtin_interrupt(BreakpointException, 3);
        set_builtin_interrupt(OverflowException, 4);
        set_builtin_interrupt(BoundRangeExceededException, 5);
        set_builtin_interrupt(InvalidOpcodeException, 6);
        set_builtin_interrupt(DeviceNotAvailableException, 7);
        set_builtin_interrupt(DoubleFaultException, 8);
        set_builtin_interrupt(CoprocessorSegmentOverrun, 9);
        set_builtin_interrupt(InvalidTSSException, 10);
        set_builtin_interrupt(SegmentationFaultException, 11);
        set_builtin_interrupt(StackFaultException, 12);
        set_builtin_interrupt(GeneralProtectionException, 13);
        set_builtin_interrupt(PageFaultException, 14);
        set_builtin_interrupt(Int15, 15);
        set_builtin_interrupt(FPUFloatingPointException, 16);
        set_builtin_interrupt(AlignmentCheckException, 17);
        set_builtin_interrupt(MachineCheckException, 18);
        set_builtin_interrupt(SIMDFloatingPointException, 19);
        set_builtin_interrupt(Int20, 20);
        set_builtin_interrupt(Int21, 21);
        set_builtin_interrupt(Int22, 22);
        set_builtin_interrupt(Int23, 23);
        set_builtin_interrupt(Int24, 24);
        set_builtin_interrupt(Int25, 25);
        set_builtin_interrupt(Int26, 26);
        set_builtin_interrupt(Int27, 27);
        set_builtin_interrupt(Int28, 28);
        set_builtin_interrupt(Int29, 29);
        set_builtin_interrupt(Int30, 30);
        set_builtin_interrupt(Int31, 31);
        set_builtin_interrupt(irq_0, 32);
        set_builtin_interrupt(irq_1, 33);
        set_builtin_interrupt(irq_2, 34);
        set_builtin_interrupt(irq_3, 35);
        set_builtin_interrupt(irq_4, 36);
        set_builtin_interrupt(irq_5, 37);
        set_builtin_interrupt(irq_6, 38);
        set_builtin_interrupt(irq_7, 39);
        set_builtin_interrupt(irq_8, 40);
        set_builtin_interrupt(irq_9, 41);
        set_builtin_interrupt(irq_10, 42);
        set_builtin_interrupt(irq_11, 43);
        set_builtin_interrupt(irq_12, 44);
        set_builtin_interrupt(irq_13, 45);
        set_builtin_interrupt(irq_14, 46);
        set_builtin_interrupt(irq_15, 47);
        set_builtin_interrupt(irq_16, 48);

        const idtr_reg idtr = {sizeof(descriptor_entry)*NUM_INTERRUPTS, IDT_BASE};

        LIDT(&idtr);
      }

      volatile descriptor_entry* descriptor(uint8_t index)
      { return ((volatile descriptor_entry*)IDT_BASE) + index; }

      interrupt_proc* interrupt(uint8_t index)
      {
#if ARCH == ARCH_X86_64
        return (interrupt_proc*)(((uint)descriptor(index)->baseLow) | (((uint)descriptor(index)->baseMiddle << 16) & ((uint)descriptor(index)->baseHigh << 32)));
#elif ARCH == ARCH_X86_32
        return (interrupt_proc*)(((uint)descriptor(index)->baseLow) | (((uint)descriptor(index)->baseHigh) << 16));
#endif
      }

      void install_interrupt(interrupt_proc interruptp, uint8_t index, uint8_t flags, uint16_t codeSelector)
      {
        volatile descriptor_entry* idt = descriptor(index);
        idt->reserved     = 0;
        idt->flags        = flags;
        idt->codeSelector = codeSelector;
        idt->baseLow      = static_cast<uint16_t>(reinterpret_cast<uint>(interruptp) & 0xFFFF);
#if ARCH == ARCH_X86_64
        idt->reserved2    = 0;
        idt->baseMiddle   = static_cast<uint16_t>((reinterpret_cast<uint>(interruptp) >> 16) & 0xFFFF);
        idt->baseHigh     = static_cast<uint32_t>((reinterpret_cast<uint>(interruptp) >> 32) & 0xFFFFFFFF);
#elif ARCH == ARCH_X86_32
        idt->baseHigh     = static_cast<uint16_t>((reinterpret_cast<uint>(interruptp) >> 16) & 0xFFFF);
#endif
      }

      uint idt_base()
      { return IDT_BASE; }

      uint interrupt_count()
      { return NUM_INTERRUPTS; }
    }
  }
