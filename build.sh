nasm bootloader/stage1/boot.asm -f bin -o build/boot.bin
nasm bootloader/stage2/main.asm -f obj -o bootloader/stage2/asm/main.obj
nasm kernel/kernel.asm -f bin -o build/kernel.bin
gcc ./tools/fat/fat.c -o build/tools/fat

# -4 for 486 cpu compatibiltity, -d3 for debugging symbols(for debugging), -s to remove stack overflow checks,
# not available in a free-standing environment, -wx to enable warnings, -ms to use the small memory model,
# -zl to not refrence to standard libraries and -zq to SHUT the compiler and only display errors and warnings.
cd ./bootloader/stage2/
wcc -4 -d3 -s -wx -ms -zl -zq -fo=/home/mgnsyt/Documents/Projects/N-OS/bootloader/stage2/c/main.obj main.c
wcc -4 -d3 -s -wx -ms -zl -zq -fo=/home/mgnsyt/Documents/Projects/N-OS/bootloader/stage2/c/stdio.obj stdio.c
nasm x86.asm -f obj -o asm/x86.obj
wlink NAME /home/mgnsyt/Documents/Projects/N-OS/build/stage2.bin FILE {/home/mgnsyt/Documents/Projects/N-OS/bootloader/stage2/asm/main.obj /home/mgnsyt/Documents/Projects/N-OS/bootloader/stage2/asm/x86.obj /home/mgnsyt/Documents/Projects/N-OS/bootloader/stage2/c/main.obj /home/mgnsyt/Documents/Projects/N-OS/bootloader/stage2/c/stdio.obj} OPTION MAP=/home/mgnsyt/Documents/Projects/N-OS/build/stage2.map @linker.lnk
cd ../../

dd if=/dev/zero of=build/os.img bs=512 count=2880
mkfs.fat -F 12 -n "N-OS" build/os.img

dd if=build/boot.bin of=build/os.img conv=notrunc
mcopy -i build/os.img build/stage2.bin "::stage2.bin"
mcopy -i build/os.img build/kernel.bin "::kernel.bin"
mcopy -i build/os.img test.txt "::test.txt"

truncate -s 1440k build/os.img
