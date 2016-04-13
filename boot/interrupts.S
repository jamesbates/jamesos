#include "gdt.h"
	.code32
	.text
	.global int_divide_zero
int_divide_zero:
	pusha
	pushf
	pushw %ds
	pushw %es
	movw $__KERN_DS, %ax
	movw %ax, %ds
	movw %ax, %es
	call divide_zero
	popw %es
	popw %ds
	popf
	popa
	iret
	.size int_divide_zero,.-int_divide_zero
	.global int_default_isr
int_default_isr:
	pusha
        pushf
        pushw %ds
        pushw %es
        movw $__KERN_DS, %ax
        movw %ax, %ds
        movw %ax, %es
        call default_isr
        popw %es
        popw %ds
        popf
        popa
        iret
        .size int_default_isr,.-int_default_isr
	.global int_irq0
int_irq0:
	pusha
	pushf
	pushw %ds
	pushw %es
	movw $__KERN_DS, %ax
	movw %ax, %ds
	movw %ax, %es
	call irq0
	popw %es
	popw %ds
	popf
	popa
	iret
	.size int_irq0,.-int_irq0
	.global int_irq1
int_irq1:
        pusha
        pushf
        pushw %ds
        pushw %es
        movw $__KERN_DS, %ax
        movw %ax, %ds
        movw %ax, %es
        call irq1
        popw %es
        popw %ds
        popf
        popa
        iret
        .size int_irq1,.-int_irq1

