/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/*
 * $Id: start.S 195 2006-10-22 20:25:10Z brcha $
 */
/** @file
 * @brief Assembly start code
 */

#include INC_ARCH(asmMacros.h)
#include INC_ARCH(mmu.h)
#include INC_ARCH(pmap.h)

/*
 * Kernel code is linked at KERNEL_BASE + 1MB, but GRUB loads it at 1MB
 * VIRT_TO_PHYS(x) maps a symbol x from link address to actual physical address
 */
#define VIRT_TO_PHYS(x) ((x) - KERNEL_BASE)

/* Temporary kernel code/data selectors */
.set CODE_SELECTOR,0x8
.set DATA_SELECTOR,0x10

#define MB_PAGE_ALIGN   (1<<0)
#define MB_MEM_INFO     (1<<1)
#define MB_MAGIC        (0x1badb002)
#define MB_FLAGS        (MB_MEM_INFO | MB_PAGE_ALIGN)
#define MB_CHECKSUM     (-(MB_MAGIC + MB_FLAGS))

/* Kernel entry point */
.text

/* Multiboot header */
.align 4
.long MB_MAGIC
.long MB_FLAGS
.long MB_CHECKSUM

.globl  _start
_start:
    movw    $0x1234,0x472   /* warm boot */

    /* Load temporary GDT and replace GRUB's one */
    lgdt    VIRT_TO_PHYS(tempGdtDesc)

    /* Reload the segmen registers with the new ones */
    movl    $DATA_SELECTOR, %eax        /* Data segment selector    */
    movw    %ax,%ds                     /* Data Segment             */
    movw    %ax,%es                     /* Extra Segment            */
    movw    %ax,%ss                     /* Stack Segment            */
    ljmp    $CODE_SELECTOR,$nextLine    /* Code Segment (must jump) */
nextLine:
    /* Clear the frame pointer (for debugging) */
    movl    $0x0,%ebp

    /* Load the temporary boot stack (+leave some space for trap frame) */
    movl    $(tempStackTop-0x40),%esp

    /* Go to C */
    pushl   %ebx			/* Multiboot info structure */
    call    init

    /* If we are here, there was an error, halt the computer */
forest:
    hlt
    jmp	    forest

.data
    /* Virtual page table */
    .globl  _vpt
    .set    _vpt, VPT
    .globl  _vpd
    .set    _vpd, (VPT + SRL(VPT, 10))

    /* Temporary stack */
    .p2align	PGSHIFT		/* Page alignment   */
    .globl	tempStack
tempStack:
    .space	KSTACKSIZE
    .globl	tempStackTop
tempStackTop:

    /* Setup the temporary GDT */
    .p2align	2		/* 4byte alignment  */
tempGDT:
    SEG_NULL					/* Null segment	*/
    SEG(STA_X|STA_R, -KERNEL_BASE, 0xffffffff)	/* Code segment	*/
    SEG(STA_W, -KERNEL_BASE, 0xffffffff)	/* Data segment */
tempGdtDesc:
    .word   0x17		    /* sizeof(tempGDT) - 1  */
    .long   VIRT_TO_PHYS(tempGDT)   /* Address of tempGDT   */

