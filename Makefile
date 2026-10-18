ASMC = nasm
CC = gcc

.PHONY: build clean run

build:
	mkdir -p build/
	mkdir -p iso/boot/grub
	$(ASMC) -f elf32 ./kernel/main.asm -o ./build/entry.o
	$(ASMC) -f elf32 ./kernel/asm/io.asm -o ./build/io.o
	$(CC) -m32 -ffreestanding -fno-pie -fno-stack-protector -O2 -Wall -Wextra -c ./kernel/kernel.c -o ./build/kernel.o
	$(CC) -m32 -nostdlib -no-pie -Wl,-T,linker.ld -Wl,--build-id=none -o ./build/kernel.elf ./build/entry.o ./build/kernel.o ./build/io.o
	cp ./build/kernel.elf ./iso/boot/kernel.elf
	cat ./configs/grub.cfg > ./iso/boot/grub/grub.cfg
	grub-mkrescue -o build/nos.iso iso/

run: build
	qemu-system-i386 -machine pc -cdrom build/nos.iso -display gtk

clean:
	rm -r build
	rm -r iso
