#define BOOT_SEG	0x07C0
#define RUN_SEG		0x0060
#define BOOT_SIGNATURE	0xAA55

	.code16
	.section .boottext,"ax",@progbits
	.global _start
_start:

	# Move ourselves to RUN_SEG
	movw $BOOT_SEG, %ax
	movw %ax, %ds
	movw $RUN_SEG, %bx
	movw %bx, %es
	xorw %si, %si
	xorw %di, %di
	movw $0x100, %cx
	cld
	rep movsw
	ljmp $RUN_SEG, $1f

	# Setup segments and stack
1:	movw %bx, %ds		# RUN_SEG
	cli
	movw %ax, %ss		# BOOT_SEG
	xorw %sp, %sp
	sti

	pushw %dx	# Save boot disk for later


	# Check MBR boot signature
	movw boot_signature, %ax
	cmpw $BOOT_SIGNATURE, %ax
	je 1f
	movw $msg_invalid_partition_table, %bx
	call puts
	jmp reboot

	# Loop through 4 partition entries, looking for a bootable one. CX = partition no.
1:	movw $partition_table, %si
1:	movb partition_status(%si), %al
	andb $0x80, %al
	jnz 1f
	incw %cx
	addw $0x10, %si
	cmpb $4, %cl
	jne 1b

	# No bootable partition found
	movw $msg_no_boot_partition, %bx
	call puts
	jmp reboot

	# Found a boot partition. Load its first sector into $BOOT_SEG:0000 and check it out
1:	movw $msg_boot_partition, %bx
	call puts
	movw %cx, %ax
	addb $'1', %al
	call putchar
	movw $msg_nl, %bx
	call puts
	movw partition_status(%si), %dx # Moves partition status and head into DL and DH
	movw partition_start_chs+chs_sector_cyl1(%si), %cx # Move sector no. and cyl. no into CL and CH
	movw $0x0201, %ax
	xorw %bx,%bx
	movw %bx, %es
	movw $BOOT_SEG, %bx
	int $0x13
	jc 1f			# Read error
	jmp _start2
1:	movw $msg_disk_read_error, %bx
	call puts
	jmp reboot
	.size _start,.-_start

	.section .disk_timestamp,"aw",@progbits
	.global disk_timestamp
disk_timestamp:
	.skip 6

	.text
	
	# Loaded partition boot sector, check its signature and jump there
_start2:        
        movw $BOOT_SEG, %bx
        movw %bx, %es
        xorw %bx, %bx
        movw %es:boot_signature(%bx), %ax
        cmpw $BOOT_SIGNATURE, %ax
        je 1f
        movw $msg_partition_invalid, %bx
        call puts
        jmp reboot

        # Partition boot sector is loaded, signature is fine; jump there!
1:      movw $msg_partition_ok, %bx
        call puts
        popw %dx                # restore backup up boot disk
        ljmp $0x0000, $(BOOT_SEG << 4)          # Many bootloaders assume 0000:7C00 rather than 07C00:0000
        .size _start2,.-_start2

reboot:
        movw $msg_reboot, %bx
        call puts
        movb $0, %ah
        int $0x16
        int $0x18
        .size reboot,.-reboot

putchar:
        pushw %bx
        pushw %ax
        movb $0x0E, %ah
        int $0x10
        popw %ax
        popw %bx
        ret
       .size putchar,.-putchar

puts:
	pushw %si
	pushw %bx
	pushw %ax
	movw %bx, %si
	movb $0x07, %bl
1:	lodsb
	cmp $0x0, %al
	je 1f
	movb $0x0E, %ah
	int $0x10
	jmp 1b
1:	popw %ax
        popw %bx
	popw %si
	ret
	.size puts,.-puts

	.section .rodata,"a",@progbits
msg_invalid_partition_table:
	.asciz "Invalid partition table\r\n"
msg_no_boot_partition:
	.asciz "No bootparttion\r\n"
msg_boot_partition:
	.asciz "Trying partition "
msg_partition_ok:
	.asciz "Boot\r\n"
msg_partition_invalid:
	.asciz "Invalid bootsector\r\n"
msg_disk_read_error:
	.asciz "Read error.\r\n"
msg_reboot:
	.asciz "Press a key."
msg_nl:
	.asciz "\r\n"

	.section .disk_signature,"aw",@progbits
	.global disk_signature, partition_table, boot_signature
disk_signature:
	.skip 6
partition_table:
	.byte 0x80
	.byte 0x00
	.byte 0x02
	.byte 0x00
	.byte 0x04
	.byte 0x00
	.byte 0x3F
	.byte 0x00
	.long 0x00000001
	.long 0x0000003E

	.skip 0x30
boot_signature:
	.word BOOT_SIGNATURE

	.struct 0
partition_status:
	.skip 1
partition_start_chs:
	.skip 3
parition_type:
	.skip 1
partition_end_chs:
	.skip 3
partition_start_lba:
	.skip 4
partition_len:
	.skip 4

	.struct 0
chs_head:
	.skip 1
chs_sector_cyl1:
	.skip 1
chs_cyl0:
	.skip 1
