#!/bin/bash

nasm -felf32 ./srcs/boot/boot.asm -o ./objs/boot.o
./utils/i386-elf-7.5.0-Linux-x86_64/bin/i386-elf-gcc -c ./srcs/kernel/kernel.c -o ./objs/kernel.o -std=gnu99 -ffreestanding
./utils/i386-elf-7.5.0-Linux-x86_64/bin/i386-elf-gcc -T linker.ld -o myos -m32 -ffreestanding -O2 -nostdlib ./objs/boot.o ./objs/kernel.o -lgcc
cp myos ./isodir/boot/myos
rm ./objs/kernel.o ./objs/boot.o
grub-mkrescue -o myos.iso isodir
kvm myos.iso
