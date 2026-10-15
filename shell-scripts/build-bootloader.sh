#/usr/bin/sh!
bytes=$(wc -c <./build/kernel.bin)
sectors=$(((bytes + 511) / 512))
nasm -f bin ./bootloader/main.asm -o ./build/boot.bin -D KERNEL_SECTOR_SIZE="$sectors"
