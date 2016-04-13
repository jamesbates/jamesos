all: mbr.img

hd: mbr.img
	dd bs=512 count=1 if=mbr.img of=hd.img conv=notrunc

mbr.img: mbr.elf
	objcopy -Obinary $< $@

mbr.elf: mbr.ld mbr.o 
	ld -Tmbr.ld -o $@ mbr.o

.S.o:
	gcc -m32 -c $<

clean: 
	rm -f mbr.o mbr.elf mbr.img
