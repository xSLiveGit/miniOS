gcc -ffreestanding -c -o kernel32.o kernel32.c
ld -Ttext 0x100000 --oformat binary -o kernel32.bin kernel32.o