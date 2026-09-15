nasm bootloader/boot.asm -f bin -o build/boot.bin
nasm kernel/kernel.asm -f bin -o build/kernel.bin
gcc ./tools/fat/fat.c -o build/tools/fat

dd if=/dev/zero of=build/os.img bs=512 count=2880
mkfs.fat -F 12 -n "N-OS" build/os.img

dd if=build/boot.bin of=build/os.img conv=notrunc
mcopy -i build/os.img build/kernel.bin "::kernel.bin"
mcopy -i build/os.img test.txt "::test.txt"

truncate -s 1440k build/os.img
