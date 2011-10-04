set disassembly-flavor intel

define hook-stop
  # x86-16 (i8086)      : CS == 0
  # x86-32 (i386)       : CS == 8
  if $cs == 0
    set architecture i8086
    # Translate segment:offset into a physical address
    printf "[%4x:%4x] ", $cs, $eip
    x/i $cs*16+$eip
  end
  if $cs == 8
    set architecture i386
    x/i $pc
  end
end

echo + target remote localhost:1234\n
target remote localhost:1234

add-symbol-file obj/mbrboot-x86-32.out 0x7C00
add-symbol-file obj/boot-x86-32.out 0xB000
add-symbol-file obj/kernel-x86-32.out 0x100000
