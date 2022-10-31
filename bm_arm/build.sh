#!/bin/sh

if [ $1 == "build" ]
    then
    arm-none-eabi-as -mcpu=arm926ej-s -g start.s -o start.o
    arm-none-eabi-gcc -c -mcpu=arm926ej-s -g main.c -o main.o
    arm-none-eabi-ld -T linker.ld main.o start.o -o app.elf
    arm-none-eabi-objcopy -O binary app.elf app.bin
fi

if [ $1 == "run" ]
    then
    arm-none-eabi-as -mcpu=arm926ej-s -g start.s -o start.o
    arm-none-eabi-gcc -c -mcpu=arm926ej-s -g main.c -o main.o
    arm-none-eabi-ld -T linker.ld main.o start.o -o app.elf
    arm-none-eabi-objcopy -O binary app.elf app.bin
    qemu-system-arm -M versatilepb -m 128M -nographic -kernel app.bin
fi
