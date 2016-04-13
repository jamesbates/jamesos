	.code16gcc
#include "gdt.h"
	.text
	.global pmjump
pmjump:
	cli			# Disable interrupts, until we set up a protected-mode IDT in interrupt.c
	movl %cr0, %eax
	orl $1, %eax
	movl %eax, %cr0		# Set CPU into 32-bit pm
	ljmp $__KERN_CS, $_start32 # jump to 32-bit entry point, in head32.S
	.size pmjump,.-pmjump
