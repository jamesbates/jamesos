	.code32
#define __ASM__ 1
#include "gdt.h"
	.text
	.global _start32
_start32:
	movw $(__KERN_DS), %ax	# set up all data segments to point to our data entry in the GDT
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	movl (basemem), %esp	# Calculate maximum usable conventional memory address (basemem was set by mem.c),
	shll $10, %esp		# and let the stack grow (down) from there.
	subl $0x7C00, %esp
	jmp main32		# bss was already zeroed in head.S, so we just jump right into C
	.size _start32,.-_start32

