
RV32GNU = riscv32-unknown-elf

AOPS = --warn --fatal-warnings -msoft-float -m32
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

all : notmain.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f *.hex
	rm -f *.vcd
	rm -f *.srec

start.o : start.s
	$(RV32GNU)-as $(AOPS) start.s -o start.o

notmain.o : notmain.c
	$(RV32GNU)-gcc $(COPS) -c notmain.c -o notmain.o

notmain.bin : memmap start.o notmain.o
	$(RV32GNU)-ld -o notmain.elf -T memmap start.o notmain.o
	$(RV32GNU)-objdump -D notmain.elf > notmain.list
	$(RV32GNU)-objcopy --srec-forceS3 notmain.elf -O srec notmain.srec
	$(RV32GNU)-objcopy notmain.elf -O binary notmain.bin

