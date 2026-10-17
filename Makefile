ASMC = nasm
CC = gcc

.PHONY: build clean run

build:
	mkdir -p build/
	$(ASMC) -f elf32 ./kernel/main.asm -o ./build/entry.o
	$(ASMC) -f elf32 ./kernel/asm/io.asm -o ./build/io.o
	$(CC) -m32 -ffreestanding -fno-pie -fno-stack-protector -O2 -Wall -Wextra -c ./kernel/kernel.c -o ./build/kernel.o
	$(CC) -m32 -nostdlib -no-pie -Wl,-T,kernel.ld -Wl,--build-id=none -o ./build/kernel.elf ./build/entry.o ./build/kernel.o ./build/io.o
	objcopy -O binary ./build/kernel.elf ./build/kernel.bin
	chmod +x ./shell-scripts/*
	./shell-scripts/build-bootloader.sh
	cat ./build/boot.bin > ./build/nos.img
	cat ./build/kernel.bin >> ./build/nos.img
	truncate -s 1474560 ./build/nos.img

run: build
	qemu-system-i386 -machine pc -drive file=build/nos.img,format=raw -display gtk

clean:
	rm -r build
