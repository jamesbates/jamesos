OUTPUT_FORMAT("elf32-i386","elf32-i386","elf32-i386")
OUTPUT_ARCH(i386)
SECTIONS {

	. = 0x0000;
	.btext		: { head.o(.text*)
	                    setup.o(.text*)
			    fd.o(.text*)
			    mbr.o(.text*) }		    
	.bdata		: { head.o(.data)
	                    setup.o(.data)
			    fd.o(.data)
			    mbr.o(.text*) }
	. = 0x01FE;
	.bootsignature	: { *(.bootsignature) }
	. = 0x0200;
	.text	: { *(.text) }
	.rodata	: { *(.rodata*) }
	.data	: { *(.data) }
	
	syssectors = ((. - 1) / 0x200);

	. = ALIGN(0x200);
	bss_start = .;
	.bss		: { *(.bss) }
	bss_len = . - bss_start;
	/DISCARD/	: { *(.eh_frame) }
}

