OUTPUT_FORMAT("elf32-i386")
ENTRY(kernel_entry)


SECTIONS
{
  . = 0x100000;

  .text ALIGN(4096) :
  {
    *(.text*)
    *(.gnu.linkonce.t*)
  }

  .rodata ALIGN(4096) :
  {
    start_ctors = .;
    *(.ctor*)
    end_ctors = .;

    start_dtors = .;
    *(.dtor*)
    end_dtors = .;

    *(.rodata*)
    *(.gnu.linkonce.r*)
  }

  .data ALIGN(4096) :
  {
    *(.data*)
    *(.gnu.linkonce.d*)
  }

  .bss ALIGN(4096) :
  {
    *(.COMMON*)
    *(.bss*)
    *(.gnu.linkonce.b*)
  }

  /DISCARD/ :
  {
    *(.comment)
    *(.eh_frame) /* You should discard this unless you're implementing runtime support for C++ exceptions. */
  }
}
