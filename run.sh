#!/bin/sh

qemu-system-i386         \
    -net none            \
    -m 8                 \
    -cdrom bin/image.iso \
    -monitor stdio       \
    #-s                  \
    #-S                  \

