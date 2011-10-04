#include "../config.h"

#if ARCH == ARCH_X86_64
.code64

#define pusha  \
  push %rax; \
  push %rbx; \
  push %rcx; \
  push %rdx; \
  push %rdi; \
  push %rsi; \
  push %rbp; \
  push %r8; \
  push %r9; \
  push %r10; \
  push %r11; \
  push %r12; \
  push %r13; \
  push %r14; \
  push %r15
   
#define popa \
  pop %r15; \
  pop %r14; \
  pop %r13; \
  pop %r12; \
  pop %r11; \
  pop %r10; \
  pop %r9; \
  pop %r8; \
  pop %rbp; \
  pop %rsi; \
  pop %rdi; \
  pop %rdx; \
  pop %rcx; \
  pop %rbx; \
  pop %rax

#else
.code32
#endif
irq_stub:
  # Disable interrupts
  cli

  # Save registers
  pusha

  # IRQ number argument
#if ARCH == ARCH_X86_64
  pushq 15*8(%rsp)
#else
  pushl 8*4(%esp)
#endif

  # Signal EOI to PIC
#if ARCH == ARCH_X86_64
  call _ZN4Apoc3X863PIC9SignalEOIEm
#else
  call _ZN3x863pic10signal_eoiEj
#endif

  # Call C++ IRQ handler
#if ARCH == ARCH_X86_64
  call _ZN4Apoc3X863IDT9HandleIRQEm
#else
  call _ZN3x863idt10handle_irqEj
#endif

  # Remove argument from stack
#if ARCH == ARCH_X86_64
  add $8, %rsp
#else
  add $4, %esp
#endif
  
  # Restore registers
  popa

  # Remove IRQ index from stack
#if ARCH == ARCH_X86_64
  add $8, %rsp
#else
  add $4, %esp
#endif

  # Reenable interrupts
  sti

  # Interrupt return
  iret

#if ARCH == ARCH_X86_64
#define IRQ(num) \
  .globl irq_ ## num; \
  irq_ ## num:; \
    pushq $num; \
    jmp irq_stub
#else
#define IRQ(num) \
  .globl irq_ ## num; \
  irq_ ## num:; \
    pushl $num; \
    jmp irq_stub
#endif

IRQ(0)
IRQ(1)
IRQ(2) 
IRQ(3) 
IRQ(4) 
IRQ(5) 
IRQ(6) 
IRQ(7) 
IRQ(8) 
IRQ(9) 
IRQ(10) 
IRQ(11) 
IRQ(12) 
IRQ(13) 
IRQ(14) 
IRQ(15) 
IRQ(16) 
