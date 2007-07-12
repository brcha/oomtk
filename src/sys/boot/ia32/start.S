/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK (http://launchpad.net/oomtk)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/** \file
 * \brief Assembly start code
 */

/* Standard assembly macros */
#include INC_ARCH(asm.h)

/*
 * This is the first file that hits the CPU after GRUB.
 *
 * In previous versions, this file was handling various descriptor tables...
 * As far as I can see now, it is not flexible to put those things into asm,
 * so now this file will do absolutely nothing more than it should be.
 *
 * And that is -> Load C++ :)
 *
 * To do so, this file must also establish the Stack (because GRUB can't do
 * that)
 */

#define MB_PAGE_ALIGN   (1<<0)
#define MB_MEM_INFO     (1<<1)
#define MB_VIDEO_MODES	(1<<2)
#define MB_MAGIC        (0x1badb002)
#define MB_FLAGS        (MB_MEM_INFO | MB_PAGE_ALIGN)
#define MB_CHECKSUM     (-(MB_MAGIC + MB_FLAGS))


/* Kernel entry point */
    .text
GEXT(start)
GEXT(_start)
    jmp over_multiboot_stuff

    /* Multiboot header */
    .align 4
multiboot_header:
    .long MB_MAGIC
    .long MB_FLAGS
    .long MB_CHECKSUM

over_multiboot_stuff:
    /* First of all, save EAX to some other register. It holds
     * Multiboot Info, but it is used by any other instruction (duh)
     */
    movl    %eax, %edx

    cld

    /* Clear the BSS Section und die Mapping Directories */
    movl    $EXT(_bss_start), %edi  /* Load destination */
    movl    $EXT(_end_maps), %ecx   /* Count revision 1 */
    subl    $EXT(_bss_start), %ecx  /* Count revision 2 */
    xorl    %eax, %eax		    /* Fill with NULL	*/
    subl    $KVA, %edi		    /* No paging set up yet! */

    /* Do the clear loop */
    rep
    stosb

    /* BSS & Mappings cleared :) */

    /* Now save the multiboot informations into kernel (in init.cc) */
    movl    $EXT(multibootSignature)-KVA, %esi
    movl    %edx, (%esi)    /* Signature */
    movl    $EXT(multibootInfo)-KVA, %esi
    movl    %ebx, (%esi)    /* Multiboot info (pointer) */

    /* Now, setup the Stack (preallocated in the linker script */
    movl    $kstack_hi - KVA, %esp

    /* Setup temporary GDT */
    lgdt    trickGDTptr - KVA

    /* Reload the segment registers with the new ones */
    movl    $0x10, %eax	    /* Data segment selector */
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %ss

    ljmp    $0x8, $nextLine  /* Code segment selector */
nextLine:
    /* Clear the frame pointer */
    movl    $0x0, %ebp

    /* Setup the stack right */
    movl    $kstack_hi, %esp

    /* Welcome to C++ */
    call    EXT(systemStartup)
    /* We should never come here... but just in case */
    jmp	    halt


    /* Global function halt() for halting the kernel... (what else) */
GEXT(halt)
1:
    cli		    /* Stop interrupts		*/
    hlt		    /* Halt			*/
    jmp	    1b	    /* And jump to halt again	*/

/* Define scripts for GDT */
#define SEG_NULL            \
          .word 0, 0;       \
          .byte 0, 0, 0, 0
#define SEG(type,base,limit)                                    \
          .word (((limit)>>12)&0xffff), ((base)&0xffff);        \
          .byte (((base)>>16)&0xff), (0x90|(type)),             \
                (0xc0|(((limit)>>28)&0xf)), (((base)>>24)&0xff)
/* Segment types are already defined in SegmentDescriptor.h */
#define segType_A     0x1
#define segType_W     0x2
#define segType_E     0x4
#define segType_R     0x2
#define segType_C     0x4
#define segType_Code  0x8
#define segType_Data  0x0

.data
    /* Many thanx to Tim Robinson for this GDT trick :) */
trickGDTptr:
    .word trickGDTend - trickGDT - 1	/* sizeof(trickGDT) - 1	*/
    .long trickGDT - KVA		/* Address of trickGDT	*/

    .p2align	2			/* 4byte alignment	*/
trickGDT:
    SEG_NULL					    /* Null segment */
    SEG(segType_Code | segType_R, -KVA, 0xffffffff) /* Code segment */
    SEG(segType_Data | segType_W, -KVA, 0xffffffff) /* Data segment */
trickGDTend:
