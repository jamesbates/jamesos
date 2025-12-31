all: hd

#floppy.img: boot.img
#	dd if=/dev/zero of=$@ count=1440 bs=1024
#	dd if=$< of=$@ bs=1 conv=nocreat,notrunc

boot.img: boot.elf
	objcopy -Obinary $< $@

boot.elf: head.o setup.o main.o tty.o fd.o lib.o a20.o mbr.o mem.o gdt.o pm.o pic_init.o pic.o head32.o main32.o tty32.o interrupts.o interrupt.o lib32.o keyb.o pit.o
	ld -Tboot.ld -o $@ head.o setup.o main.o tty.o fd.o lib.o a20.o mbr.o mem.o gdt.o pm.o pic_init.o pic.o head32.o main32.o tty32.o interrupts.o interrupt.o lib32.o keyb.o pit.o

lib32.o: lib.c
	$(CC) -c -m32 -Os -fno-asynchronous-unwind-tables -fno-exceptions -fno-pie -ffreestanding -fomit-frame-pointer -D__LIB32__ lib.c -o lib32.o

.S.o:
	$(CC) -c -m32 $<

.c.o:
	$(CC) -S -m32 -Os -fno-asynchronous-unwind-tables -fno-exceptions -fno-pie -ffreestanding -fomit-frame-pointer $<
	$(CC) -c -m32 -Os -fno-asynchronous-unwind-tables -fno-exceptions -fno-pie -ffreestanding -fomit-frame-pointer $<

clean:
	rm -f boot.img floppy.img *.o *.s *.elf

#hd.img: boot.img ../mbr/mbr.img
#	dd if=/dev/zero of=hd.img bs=512 seek=20480 count=0
#	/sbin/fdisk hd.img < fdisk.cmd
#	dd if=boot.img seek=2049 of=hd.img bs=512 conv=nocreat,notrunc
#	dd if=../mbr/mbr.img of=hd.img bs=1 conv=notrunc

hd: boot.img
	dd if=$< seek=2048 bs=512 of=hd.img conv=nocreat,notrunc

run: hd
	qemu-system-i386 -hda hd.img -m 32M # -display curses
