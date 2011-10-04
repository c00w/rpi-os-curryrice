.intel_syntax noprefix

#include "../config.h"
#include "../x86/x86.h"

# GDT 64 segment selectors
.set GDT64_DSEG_SEL, 8
.set GDT64_CSEG_SEL, 16

.set KERNEL_ADDRESS, 0x100000

# BOOT:
#   Load kernel
#   Create simple identity paging table
#   Transition to 64 bit?
#   Call kernel

.globl protected_boot
protected_boot:
  .code32
#if ARCH == ARCH_X86_64
  # Build flat 2MB paging system (consecutive in memory)
  # [00008000 - 00009000] (4096 B) - PML4 *ALIGN4KB
  # [00009000 - 0000A000] (4096 B) - PDPT *ALIGN4KB
  # [0000A000 - 0000B000] (4096 B) - PD *ALIGN4KB

  mov edi, 0x00008000   # Start of tables
  cld                   # String operations add to edi 

  # Build PML4
  # PHYSADDR-PDPT = 0x9000 =                              10010000 00000000
  # PML4E0 = PHYSADDR-PDPT | 1111 =
  # 00000000 00000000 00000000 00000000 00000000 00000000 10010000 00001111

  # First entry is valid
  mov eax, 0b00000000000000001001000000001111
  stosd

  # Write rest of PML4 to be NULL
  xor eax, eax
  mov ecx, 1023
  rep stosd

  # Build PDPT

  # PHYSADDR-PD = 0xA000 =                                10100000 00000000
  # PDPTE0 = PHYSADDR-PD | 1111 =
  # 00000000 00000000 00000000 00000000 00000000 00000000 10100000 00001111

  # First entry is valid
  mov eax, 0b00000000000000001010000000001111
  stosd

  # Write rest of PDPT to be NULL
  xor eax, eax
  mov ecx, 1023
  rep stosd

  # Build flat PD

  # PHYSADDR-PAGE = 0x200000*N
  # PDE0 = PHYSADDR-PAGE | 110001111 =
  # 00000000 00000000 00000000 00000000 00000000 00000000 00000001 10001111
  sub edi, 8
  mov ecx, -0x200000

write_pde:
  add edi, 8                                  # Next PDE
  add ecx, 0x200000                           # Increment page address by 2 MB

  mov eax, 0b00000000000000000000000110001111 # Build source PDE
  or eax, ecx

  mov DWORD PTR [edi], eax                    # Write PDE
  mov DWORD PTR [edi + 4], 0

  cmp edi, 0x0000A000 + 8*511                 # End of PD?
  jne write_pde                               # Not end of PD

  # Enable physical-address extensions (PAE)
  mov eax, cr4
  or eax, 0b100000
  mov cr4, eax

  # Point cr3 at PML4
  mov eax, 0x8000
  mov cr3, eax

  # Enable long mode (set EFER.LME=1)
  mov ecx, 0xC0000080                         # EFER MSR number
  rdmsr                                       # Read EFER
  or eax, 0b100000001                         # LME amd SYSCALL/SYSRET
  wrmsr                                       # Write EFER
  
  # Enable paging AND protected mode to activate long mode (set CR0.PG=1)
  mov eax, cr0                                # Read CR0
  or eax, 0b10000000000000000000000000000001  # enable paging + pmode
  mov cr0, eax                                # Write CR0

  lgdt x86_64_gdt_desc

  jmp GDT64_CSEG_SEL, x86_64_boot

  .code64
x86_64_boot:

  # Prepare stack
  mov rbp, 0x3FFFFF
  mov rsp, rbp

  # Call kernel
  call KERNEL_ADDRESS

  jmp $

#else
  .code32

  # Call kernel
  call KERNEL_ADDRESS

  jmp $

#endif

#if ARCH == ARCH_X86_64

# Flat x86-64 GDT
.p2align 3
x86_64_gdt:
  SEG_NULL                            # null seg
  SEG64(STA_W, 0x0, 0xFFFFFFFF)       # data seg
  SEG64(STA_X|STA_R, 0x0, 0xFFFFFFFF) # code seg

# x86-64 GDT descriptor
x86_64_gdt_desc:
  .word 8*3 - 1                       # sizeof(x86_64_gdt) - 1
  .quad x86_64_gdt                    # address x86_64_gdt

#endif
