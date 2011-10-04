.intel_syntax noprefix

#include "../config.h"
#include "../x86/x86.h"

.set QEMU, 1

# GDT 32 segment selectors
.set GDT32_CSEG_SEL, 8
.set GDT32_DSEG_SEL, 16

# Protected mode enable flag
.set CR0_PE_ON, 1

.set BOOT_ADDRESS, 0xB000
.set BOOT_ADDRESS_16SEG, 0
.set BOOT_ADDRESS_16OFF, 0xB000
.set BOOT_NUM_SECTORS, 1
.if QEMU
.set BOOT_START_SECTOR, 1
.else
.set BOOT_START_SECTOR, 480150720 + 1
.endif

.set KERNEL_ADDRESS, 0x100000
.set KERNEL_ADDRESS_16SEG, 0xFFFF
.set KERNEL_ADDRESS_16OFF, 0x10
.set KERNEL_NUM_SECTORS, 90
.set KERNEL_START_SECTOR, BOOT_START_SECTOR + BOOT_NUM_SECTORS

# MBRBOOT:
#   Enables A20
#   Maps memory
#   Loads boot, and kernel
#   Transition to 32 bit protected mode
#   Call boot
.globl real_boot
real_boot:
  .code16
  cli
  cld

  xor ax, ax
  mov ds, ax
  mov ss, ax
  mov es, ax

  # Enable A20:
  #   For backwards compatibility with the earliest PCs, physical
  #   address line 20 is tied low, so that addresses higher than
  #   1MB wrap around to zero by default.  This code undoes this.
seta20.1:
  in al, 0x64   # Wait for not busy
  test al, 0x2
  jnz seta20.1

  mov al, 0xd1  # 0xd1 -> port 0x64
  out 0x64, al 

seta20.2:
  in al, 0x64   # Wait for not busy
  test al, 0x2
  jnz seta20.2

  mov al, 0xdf  # 0xdf -> port 0x60
  outb 0x60, al

  # Detect memory
  #   INT 15h, EAX=E820h
  #   Destination = ES:DI
  mov ax, 208
  mov es, ax

  xor ebx, ebx
  xor edi, edi
  mov edx, 0x534D4150

  mov eax, 0x7bfb
  mov dword ptr [eax], 0

mem_map:
  mov ecx, 24
  mov eax, 0xE820
  int 0x15

  mov esi, 0x7bfb
  inc dword ptr [esi]

  # Error handling
  jc success
  mov edx, 0x0534D4150    # Some BIOSes trash register

  cmp eax, edx            # On success, eax must have been reset to "SMAP"
  jne failed

  # Check if end of list
  cmp ebx, 0              # If ebx resets to 0, list is complete
  je success
  
  # No error
  add di, 24
  jmp mem_map

failed:
  mov dx, 0x3F8
  mov al, 0x46
  out dx, al
  jmp failed

success:

  #
  # Real Mode read hd
  #
  mov dl, 0x80 # Drive number (0x80 for hard disk)
  mov ah, 0x42 # Function selector (AH = 42h Extended Read)
  mov si, offset boot_dap
  int 0x13

  mov dl, 0x80 # Drive number (0x80 for hard disk)
  mov ah, 0x42 # Function selector (AH = 42h Extended Read)
  mov si, offset kernel_dap
  int 0x13

  # Switch from real to protected mode, using a bootstrap x86_32_gdt and segment translation that makes virtual addresses identical to their physical addresses, so that the effective memory map does not change during the switch.
  lgdt x86_32_gdt_desc
  mov eax, cr0
  or eax, CR0_PE_ON
  mov cr0, eax
  
  # Switch processor into 32-bit mode.
  jmp GDT32_CSEG_SEL, x86_32_boot

  .code32 # Assemble for 32-bit mode
x86_32_boot:
  # Set up the protected-mode data segment registers
  mov ax, GDT32_DSEG_SEL    # Our data segment selector
  mov ds, ax                # -> DS: Data Segment
  mov es, ax                # -> ES: Extra Segment
  mov fs, ax                # -> FS
  mov gs, ax                # -> GS
  mov ss, ax                # -> SS: Stack Segment
  
  # Initialize stack 
  mov ebp, 0x3FFFFF
  mov esp, ebp

  # Jump to boot
  jmp BOOT_ADDRESS

#
# DATA
#

.balign 4
boot_dap:
.byte 0x10                 # Size of packet
.byte 0                    # Unused
.byte BOOT_NUM_SECTORS     # Num sectors
.byte 0                    # Unused
.word BOOT_ADDRESS_16OFF   # Offset
.word BOOT_ADDRESS_16SEG   # Segment
.quad BOOT_START_SECTOR    # Sector start

.balign 4
kernel_dap:
.byte 0x10                 # Size of packet
.byte 0                    # Unused
.byte KERNEL_NUM_SECTORS   # Num sectors
.byte 0                    # Unused
.word KERNEL_ADDRESS_16OFF # Offset
.word KERNEL_ADDRESS_16SEG # Segment
.quad KERNEL_START_SECTOR  # Sector start

# Flat x86-32 GDT
.balign 4
x86_32_gdt:
  SEG_NULL                            # null seg
  SEG32(STA_X|STA_R, 0x0, 0xFFFFFFFF) # code seg
  SEG32(STA_W, 0x0, 0xFFFFFFFF)       # data seg

# x86-32 GDT descriptor
x86_32_gdt_desc:
  .word 8*3 - 1                       # sizeof(x86_32_gdt) - 1
  .long x86_32_gdt                    # address x86_32_gdt
