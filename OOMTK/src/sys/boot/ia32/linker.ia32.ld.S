/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/*
 * $Id: linker.ia32.ld 196 2006-10-23 13:29:52Z brcha $
 */
/** @file
 * @brief Linker script
 */
ENTRY(_start)
OUTPUT_FORMAT("elf32-i386")

SECTIONS
{
    . = KVA + 0x100000; /* Kernel start at Kernel Virtual Address + 1MB */

    /* Standard text region (code) */
    _text = .;
    .text : AT(ADDR(.text) - KVA)
    {
	*(.text)
	_rodata = .;
	*(.rodata)
	*(.rodata.*)

        /* C++ Support - constructors & destructors */

        /* Constructors */
        . = ALIGN(4);

        __ctors_start__ = .;
        *(SORT(.ctor*))
        QUAD(0)
        __ctors_end__ = .;

        . = ALIGN(4);

        __dtors_start__ = .;
        *(SORT(.dtor*))
        QUAD(0)
        __dtors_end__ = .;

        . = ALIGN(4);

	_end_text = .;
    } = 0x9090

    /* Page aligned data, such as IDT */
    . = ALIGN(PAGE_SIZE);
    _pagedata = .;
    .pagedata : AT(ADDR(.pagedata) - KVA)
    {
	*(.pagedata)
    }
    _end_pagedata = .;

    /* The rest of the data */
    _data = .;
    .data : AT(ADDR(.data) - KVA)
    {
	*(.data)
    }

    . = ALIGN(CACHE_LINE_SIZE);
    .data.percpu :
    {
	*(.data.percpu)
	. = ALIGN(CACHE_LINE_SIZE);
    }
    _end_data = .;

    /* BSS (Uninitialized data => to be zeroed) */
    _bss_start = .;
    .bss : AT(ADDR(.bss) - KVA)
    {
	*(.bss)
    }
    _bss_end = .;

    _begin_maps = .;
    /* Master paging directory PTE & PAE versions */
    MasterPageDirPTE = .;
    MasterPageDirPAE = .;
    . = . + PAGE_SIZE;

    /* Kernel page table - 2MB for PAE or 4MB for PTE */
    . = ALIGN(PAGE_SIZE);
    KernelPageTable = .;
    . = . + PAGE_SIZE;

    /* Kernel Stack */
    . = ALIGN(KSTACK_SIZE);
    kstack_lo = .;
    . = . + KSTACK_SIZE;
    kstack_hi = .;

    _end_maps = .;

    /* Discard debugging, relocations, exceptions, etc */
    debug :
    {
	*(.stab*)
	*(.debug*)
	*(.comment*)
	*(.shstrtab*)
	*(.symtab*)
	*(.strtab*)
    }

    /DISCARD/ :
    {
	*(.rel.*)
	*(.eh_frame)
    }
}
