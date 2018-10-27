nasm -f bin -o asm.o mbr.asm
cc1.exe -ffreestanding -m64 kernel32.c -o kernel32.o -Wall -O0 -masm=intel
as.exe --64 kernel32.o -o kernel32.out -msyntax=intel
ld.exe -Ttext 0x100000 --oformat binary kernel32.out -o kernel32.bin -m elf_x86_64  -O0