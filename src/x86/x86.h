#pragma once
#ifndef __ASSEMBLER__
#include "types.h"
#include "pic.h"
#include "idt.h"
#include "gdt.h"
#include "pit.h"
#include "keyboard.h"

namespace x86
{
  //
  // Assembly instructions
  //
  static __inline void IOWait(void);
  static __inline void STI(void) __attribute__((always_inline));
  static __inline void CLI(void) __attribute__((always_inline));
  static __inline void INT3(void) __attribute__((always_inline));
  static __inline uint8_t INB(int port) __attribute__((always_inline));
  static __inline void insb(int port, void *addr, int cnt) __attribute__((always_inline));
  static __inline uint16_t INW(int port) __attribute__((always_inline));
  static __inline void insw(int port, void *addr, int cnt) __attribute__((always_inline));
  static __inline uint32_t inl(int port) __attribute__((always_inline));
  static __inline void INSL(int port, void *addr, int cnt) __attribute__((always_inline));
  static __inline void OUTB(int port, uint8_t data) __attribute__((always_inline));
  static __inline void outsb(int port, const void *addr, int cnt) __attribute__((always_inline));
  static __inline void OUTW(int port, uint16_t data) __attribute__((always_inline));
  static __inline void outsw(int port, const void *addr, int cnt) __attribute__((always_inline));
  static __inline void OUTSL(int port, const void *addr, int cnt) __attribute__((always_inline));
  static __inline void outl(int port, uint32_t data) __attribute__((always_inline));
  static __inline void invlpg(void *addr) __attribute__((always_inline));
  static __inline void LIDT(const void *p) __attribute__((always_inline));
  static __inline void lldt(uint16_t sel) __attribute__((always_inline));
  static __inline void ltr(uint16_t sel) __attribute__((always_inline));
  static __inline void lcr0(uint32_t val) __attribute__((always_inline));
  static __inline uint32_t rcr0(void) __attribute__((always_inline));
  static __inline uint32_t rcr2(void) __attribute__((always_inline));
  static __inline void lcr3(uint32_t val) __attribute__((always_inline));
  static __inline uint32_t rcr3(void) __attribute__((always_inline));
  static __inline void lcr4(uint32_t val) __attribute__((always_inline));
  static __inline uint32_t rcr4(void) __attribute__((always_inline));
  static __inline void tlbflush(void) __attribute__((always_inline));
  static __inline uint32_t read_eflags(void) __attribute__((always_inline));
  static __inline void write_eflags(uint32_t eflags) __attribute__((always_inline));
  static __inline uint32_t read_ebp(void) __attribute__((always_inline));
  static __inline uint32_t read_esp(void) __attribute__((always_inline));
  static __inline void CPUID(uint32_t info, uint32_t *eaxp, uint32_t *ebxp, uint32_t *ecxp, uint32_t *edxp);

  static __inline void IOWait(void)
  { __asm __volatile("outb %%al, $0x80" : : "a"(0)); }

  static __inline void STI(void)
  { __asm __volatile("sti"); }

  static __inline void CLI(void)
  { __asm __volatile("cli"); }

  static __inline void INT3(void)
  { __asm __volatile("int3"); }

  static __inline uint8_t INB(int port)
  {
    uint8_t data;
    __asm __volatile("inb %w1,%0" : "=a" (data) : "d" (port));
    return data;
  }

  static __inline void insb(int port, void *addr, int cnt)
  {
    __asm __volatile("cld\n\trepne\n\tinsb"            :
		     "=D" (addr), "=c" (cnt)           :
		     "d" (port), "0" (addr), "1" (cnt) :
		     "memory", "cc");
  }

  static __inline uint16_t INW(int port)
  {
	  uint16_t data;
	  __asm __volatile("INW %w1,%0" : "=a" (data) : "d" (port));
	  return data;
  }

  static __inline void insw(int port, void *addr, int cnt)
  {
	  __asm __volatile("cld\n\trepne\n\tinsw"			:
			   "=D" (addr), "=c" (cnt)		:
			   "d" (port), "0" (addr), "1" (cnt)	:
			   "memory", "cc");
  }

  static __inline uint32_t inl(int port)
  {
	  uint32_t data;
	  __asm __volatile("inl %w1,%0" : "=a" (data) : "d" (port));
	  return data;
  }

  static __inline void INSL(int port, void *addr, int cnt)
  {
	  __asm __volatile("cld\n\trepne\n\tinsl"			:
			   "=D" (addr), "=c" (cnt)		:
			   "d" (port), "0" (addr), "1" (cnt)	:
			   "memory", "cc");
  }

  static __inline void OUTB(int port, uint8_t data)
  {
	  __asm __volatile("outb %0,%w1" : : "a" (data), "d" (port));
  }

  static __inline void outsb(int port, const void *addr, int cnt)
  {
	  __asm __volatile("cld\n\trepne\n\toutsb"		:
			   "=S" (addr), "=c" (cnt)		:
			   "d" (port), "0" (addr), "1" (cnt)	:
			   "cc");
  }

  static __inline void OUTW(int port, uint16_t data)
  {
	  __asm __volatile("outw %0,%w1" : : "a" (data), "d" (port));
  }

  static __inline void outsw(int port, const void *addr, int cnt)
  {
	  __asm __volatile("cld\n\trepne\n\toutsw"		:
			   "=S" (addr), "=c" (cnt)		:
			   "d" (port), "0" (addr), "1" (cnt)	:
			   "cc");
  }

  static __inline void OUTSL(int port, const void *addr, int cnt)
  {
	  __asm __volatile("cld\n\trepne\n\toutsl"		:
			   "=S" (addr), "=c" (cnt)		:
			   "d" (port), "0" (addr), "1" (cnt)	:
			   "cc");
  }

  static __inline void outl(int port, uint32_t data)
  {
	  __asm __volatile("outl %0,%w1" : : "a" (data), "d" (port));
  }

  static __inline void invlpg(void *addr)
  { 
	  __asm __volatile("invlpg (%0)" : : "r" (addr) : "memory");
  }  

  static __inline void LIDT(const void *p)
  {
	  __asm __volatile("lidt (%0)" : : "r" (p));
  }

  static __inline void lldt(uint16_t sel)
  {
	  __asm __volatile("lldt %0" : : "r" (sel));
  }

  static __inline void ltr(uint16_t sel)
  {
	  __asm __volatile("ltr %0" : : "r" (sel));
  }

  static __inline void lcr0(uint32_t val)
  {
	  __asm __volatile("movl %0,%%cr0" : : "r" (val));
  }

  static __inline uint32_t rcr0(void)
  {
	  uint32_t val;
	  __asm __volatile("movl %%cr0,%0" : "=r" (val));
	  return val;
  }

  static __inline uint32_t rcr2(void)
  {
	  uint32_t val;
	  __asm __volatile("movl %%cr2,%0" : "=r" (val));
	  return val;
  }

  static __inline void lcr3(uint32_t val)
  {
	  __asm __volatile("movl %0,%%cr3" : : "r" (val));
  }

  static __inline uint32_t rcr3(void)
  {
	  uint32_t val;
	  __asm __volatile("movl %%cr3,%0" : "=r" (val));
	  return val;
  }

  static __inline void lcr4(uint32_t val)
  {
	  __asm __volatile("movl %0,%%cr4" : : "r" (val));
  }

  static __inline uint32_t rcr4(void)
  {
	  uint32_t cr4;
	  __asm __volatile("movl %%cr4,%0" : "=r" (cr4));
	  return cr4;
  }

  static __inline void tlbflush(void)
  {
	  uint32_t cr3;
	  __asm __volatile("movl %%cr3,%0" : "=r" (cr3));
	  __asm __volatile("movl %0,%%cr3" : : "r" (cr3));
  }

  static __inline uint32_t read_eflags(void)
  {
	  uint32_t eflags;
	  __asm __volatile("pushfl; popl %0" : "=r" (eflags));
	  return eflags;
  }

  static __inline void write_eflags(uint32_t eflags)
  {
	  __asm __volatile("pushl %0; popfl" : : "r" (eflags));
  }

  static __inline uint32_t read_ebp(void)
  {
	  uint32_t ebp;
	  __asm __volatile("movl %%ebp,%0" : "=r" (ebp));
	  return ebp;
  }

  static __inline uint32_t read_esp(void)
  {
	  uint32_t esp;
	  __asm __volatile("movl %%esp,%0" : "=r" (esp));
	  return esp;
  }

  static __inline void CPUID(uint32_t info, uint32_t *eaxp, uint32_t *ebxp, uint32_t *ecxp, uint32_t *edxp)
  {
    uint32_t eax, ebx, ecx, edx;
    asm volatile("cpuid" 
    : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
    : "a" (info));

    if (eaxp)
      *eaxp = eax;
    if (ebxp)
      *ebxp = ebx;
    if (ecxp)
      *ecxp = ecx;
    if (edxp)
      *edxp = edx;
  }
}

#else

#define SEG_NULL						\
	.word 0, 0;						\
	.byte 0, 0, 0, 0

#define SEG32(type,base,lim)				\
	.word (((lim) >> 12) & 0xffff), ((base) & 0xffff);	\
	.byte (((base) >> 16) & 0xff), (0x90 | (type)),		\
		(0xC0 | (((lim) >> 28) & 0xf)), (((base) >> 24) & 0xff)

#define SEG64(type,base,lim)				\
	.word (((lim) >> 12) & 0xffff), ((base) & 0xffff);	\
	.byte (((base) >> 16) & 0xff), (0x90 | (type)),		\
		(0xE0 | (((lim) >> 28) & 0xf)), (((base) >> 24) & 0xff)

#define STA_X 0x8 // Executable segment
#define STA_E 0x4 // Expand down (non-executable segments)
#define STA_C 0x4 // Conforming code segment (executable only)
#define STA_W 0x2 // Writeable (non-executable segments)
#define STA_R 0x2 // Readable (executable segments)
#define STA_A 0x1 // Accessed
#endif
