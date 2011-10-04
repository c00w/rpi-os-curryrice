#
# VARIABLES
# 
SRCDIR = src
OBJDIR = obj
BINDIR = bin
UTLDIR = utils

DISKMBRPATH = utils/diskmbr/diskmbr
GRUB2SCRIPTPATH = /etc/grub.d/90_apoc

CCFLAGS = -I $(SRCDIR) -std=c++0x -g -O0 -nostdinc -Wall -static -fno-common -fno-exceptions -fno-stack-protector -fno-non-call-exceptions -fno-weak -fno-builtin -ffreestanding -nostdlib -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-3dnow -fno-rtti -c

X86_32_CCFLAGS = $(CCFLAGS) -m32 -mtune=i386 -D ARCH=ARCH_X86_32

LDFLAGS = -nostdlib -nodefaultlibs

X86_32_LDFLAGS = $(LDFLAGS) -melf_i386 -g

LD = ld
CC = gcc
OC = objcopy

#
# TARGETS
#
all: x86-32
x86: x86-32
x86-32: x86-32-base x86-32-debug
x86-32-base: $(OBJDIR)/kernel-x86-32 $(OBJDIR)/mbrboot-x86-32 $(OBJDIR)/boot-x86-32
x86-32-debug: $(BINDIR)/apoc-x86-32.img
utils: $(UTLDIR)/diskmbr/diskmbr

$(BINDIR)/apoc-x86-32.img: x86-32-base utils
	@echo + $@
	@$(DISKMBRPATH) -create $(OBJDIR)/mbrboot-x86-32 > $(OBJDIR)/mbr-x86-32
	@cp $(OBJDIR)/mbr-x86-32 $@
	@dd if=$(OBJDIR)/boot-x86-32 of=$@ oflag=append count=1 conv=notrunc,sync > /dev/null 2>&1
	@dd if=$(OBJDIR)/kernel-x86-32 of=$@ oflag=append conv=notrunc,sync > /dev/null 2>&1
	@dd if=/dev/zero of=$@ oflag=append count=500 conv=notrunc,sync > /dev/null 2>&1

clean:
	@rm -f $(OBJDIR)/* $(BINDIR)/*
	@rm -f tags
	@rm -f utils/diskmbr/diskmbr

debug-x86-32: x86-32-debug
	qemu -bios utils/qemu-0.12.5/pc-bios/bios.bin -vga std -k en-us -hda $(BINDIR)/apoc-x86-32.img -serial stdio

debug-gdb-x86-32: x86-32-debug
	qemu -bios utils/qemu-0.12.5/pc-bios/bios.bin -vga std -k en-us -hda $(BINDIR)/apoc-x86-32.img -serial stdio -s -S

debug-gdb: debug-gdb-x86-32
debug: debug-x86-32

install-grub2-x86-32: $(BINDIR)/apoc-x86-32.img
	@echo "Apoc partition file:"; \
	read device_path; \
	echo Installing APOC to $$device_path...; \
	dd if=$(BINDIR)/apoc-x86-32.img of=$$device_path;
	@echo "Apoc partition index:"; \
	read device_index; \
	echo exec\ tail\ --lines=4\ '$$''0' > $(GRUB2SCRIPTPATH) && echo "menuentry \"Apoc\" {" >> $(GRUB2SCRIPTPATH) && echo "set root=(hd0,$$device_index)" >> $(GRUB2SCRIPTPATH) && echo "chainloader +1" >> $(GRUB2SCRIPTPATH) && echo "}" >> $(GRUB2SCRIPTPATH)
	@chmod +x $(GRUB2SCRIPTPATH)
	@echo "Updating GRUB..."
	@update-grub

install-grub2: install-grub2-x86-32

include src/Makefrag
include src/boot/Makefrag
include utils/diskmbr/Makefrag
