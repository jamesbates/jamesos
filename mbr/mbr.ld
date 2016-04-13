OUTPUT_FORMAT("elf32-i386","elf32-i386","elf32-i386")
ENTRY(_start)
OUTPUT_ARCH(i386)
SECTIONS {

	. = 0x0000;
	.boottext	: { *(.boottext*) }

	. = 0x00DA;
	.disk_timestamp : { *(.disk_timestamp*) }
	.text		: { *(.text*) }
	.rodata		: { *(.rodata*) }
	.data		: { *(.data*) }
	prog_end = .;
	. = 0x01B8;
	.disk_signature	: { *(.disk_signature) }

	mbr_end = .;

	.bss		: { *(.bss*) }
	/DISCARD/	: { *(.eh_frame) }
}
ASSERT (mbr_end == 0x200, "Invalid mbr data")
ASSERT (prog_end <= 0x1B8, "Program too large")
