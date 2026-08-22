GCC = /usr/opt/cross-compiler/bin/i686-elf-gcc
LD = /usr/opt/cross-compiler/bin/i686-elf-ld
CFLAGS = -ffreestanding -fno-stack-protector -nostdlib -Wall -Wextra
LDFLAGS = --no-warn-rwx-segments

BUILD_DIR = build

OBJS = $(BUILD_DIR)/kmain.o $(BUILD_DIR)/vga.o $(BUILD_DIR)/gdt.o $(BUILD_DIR)/boot.o $(BUILD_DIR)/gdtasm.o $(BUILD_DIR)/keyboard.o $(BUILD_DIR)/idtasm.o $(BUILD_DIR)/idt.o

all: kernel boot link

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

kernel: | $(BUILD_DIR)
	$(GCC) $(CFLAGS) -c -g kmain.c -o $(BUILD_DIR)/kmain.o
	$(GCC) $(CFLAGS) -c -g vga.c -o $(BUILD_DIR)/vga.o
	$(GCC) $(CFLAGS) -c -g gdt.c -o $(BUILD_DIR)/gdt.o
	$(GCC) $(CFLAGS) -c -g idt.c -o $(BUILD_DIR)/idt.o
	$(GCC) $(CFLAGS) -c -g input/keyboard.c -o $(BUILD_DIR)/keyboard.o
	nasm -f elf32 gdt.asm -o $(BUILD_DIR)/gdtasm.o
	nasm -f elf32 idt.asm -o $(BUILD_DIR)/idtasm.o

boot: | $(BUILD_DIR)
	nasm -f elf32 boot.asm -o $(BUILD_DIR)/boot.o

link: kernel boot
	$(LD) -T ../../linker.ld -o $(BUILD_DIR)/kernel $(OBJS) $(LDFLAGS)
	cp $(BUILD_DIR)/kernel OS/boot/kernel
	grub-mkrescue -o kernel.iso OS/
