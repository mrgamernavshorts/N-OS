# N-OS

A Operating system built from the ground up!

(It is being actively working on, so it's incomplete. I just wanted to mark a beginning point, so I made a Repo for it :D)

# Build Instructions


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

Run using Qemu _(If not already installed, go and watch a tutorial to install it :D)_:
```Bash
qemu-system-i386 -drive format=raw,file=build/main.img,if=floppy -display gtk
```
If the above command is giving your errors, install the `qemu-desktop` package from your package manager.
