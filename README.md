# N-OS

A x86 legacy boot Operating system built from the ground up!

(It is being actively worked on, so it's incomplete. I just wanted to mark a beginning point, so I made a Repo for it :D)

Now it works on bare metal!

# Build Instructions

## Dependencies

* mcopy
* Qemu (Or any VM software)
* Watcom (C compiler, should be wcc _(The 16-bit compiler)_))

## Procedure

Clone the Repo and CD into it:
```Bash
git clone https://github.com/mrgamernavshorts/N-OS
cd N-OS
```


Create a `build` directory, Make the `build.sh` file executable and run it:
```Bash
mkdir build
chmod +x build.sh
./build.sh
```

This will compile the `main.img` file in `build/`.

## Run using Qemu

Run using Qemu _(If not already installed, go and watch a tutorial to install it :D)_:
```Bash
qemu-system-i386 -drive format=raw,file=build/main.img,if=floppy -display gtk
```
If the above command is giving your errors, install the `qemu-desktop` package from your package manager.

## Run on bare metal

### On Linux

To flash the image using `dd`:
```Bash
dd if=build/os.img of=/dev/your_flash_drive status=progress
```

You can see all the storage devices by running `lsblk` in the terminal

### On Windows

You can flash the `build/os.img` through [Rufus](https://rufus.ie/en/).
