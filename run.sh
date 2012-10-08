#!/bin/sh

qemu-system-i386        \
    -net none           \
    -m 16               \
    -cdrom bin/image.iso\

