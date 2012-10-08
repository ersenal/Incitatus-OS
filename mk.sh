#!/bin/sh
CFlags="-nostdlib -fno-builtin -fno-stack-protector -O2 -Wall -Wextra -Werror -std=gnu99 -I kernel/include/"

nasm -f elf -o start.o  kernel/src/Start.s

/usr/local/cross/bin/i586-elf-gcc -o kernel.o -c kernel/src/Kernel.c $CFlags

/usr/local/cross/bin/i586-elf-ld -Map bin/Mem.map -T kernel/src/Linker.ld -o bootloader/kernel        start.o \
                                                                                                      kernel.o \

genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o bin/image.iso bootloader/

rm *.o
rm bootloader/kernel
