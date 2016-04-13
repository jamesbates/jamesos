.code16
.text
.global _start

_start:
	ljmp $0x07C0, $1f	# Normalize start address
1:
	movw %cs, %ax		# Set es to cs (i.e. 0x07C0)
	movw %ax, %es
	movw $bss_start, %di    # clear bss
	xorb %al, %al
	movw $bss_len, %cx
	rep stosb
	movw %ds, %ax           # store pointer to mbr partition entry (at ds:si) for later use by mbr.c
	movw %ax, %es:s_boot_part
	movw %es, %ax		# point ds to es (i.e. 0x07C0)
	movw %ax, %ds
	cli
	movw %ax, %ss		# Set up stack just under bootsector
	xorl %esp, %esp
	sti
	movb %dl, drive		# store startup disk for later use by fd.c
	movw %si, o_boot_part	# store pointer to mbr boot partition entry
	jmp setup		# Jump to C entry point in setup.c
.size	_start,.-_start

.section ".bootsignature","a"
	.byte 0x55
	.byte 0xAA
