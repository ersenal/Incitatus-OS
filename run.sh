#!/bin/sh

qemu-system-i386        \
    -net none           \
    -m 4                \
    -cdrom bin/image.iso\
    -monitor stdio      \

