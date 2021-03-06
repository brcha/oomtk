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
 * \brief Linker script
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

    /* Page aligned data	*/
    . = ALIGN(PAGE_SIZE);
    _pagedata = .;
    .pagedata : AT(ADDR(.pagedata) - KVA)
    {
	*(.pagedata)
    }
    _end_pagedata = .;

    /* Traps & interrupts	*/
    . = ALIGN(PAGE_SIZE);
    _traps_data = .;
    .traps.data : AT(ADDR(.traps.data) - KVA)
    {
	*(.traps.data)
    }
    _end_traps_data = .;

    . = ALIGN(PAGE_SIZE);
    _traps_text = .;
    .traps.text : AT(ADDR(.traps.text) - KVA)
    {
	*(.traps.text)
    }
    _end_traps_text = .;

    /* The rest of the data	*/
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

    /* Begin mappings */
    . = ALIGN(PAGE_SIZE);
    _begin_maps = .;
    /* Master paging directory */
    MasterPageDir = .;
    . = . + PAGE_SIZE;

    /* Kernel page table - 2MB for PAE or 4MB for PTE */
    . = ALIGN(PAGE_SIZE);
    KernPageTable = .;
    . = . + PAGE_SIZE;

    /* Transient mappings - idea from Coyotos */
    . = ALIGN(PAGE_SIZE);
    TransMap = .;
    . = . + PAGE_SIZE;

    /* Kernel Stack */
    . = ALIGN(KSTACK_SIZE);
    kstack_lo = .;
    . = . + KSTACK_SIZE;
    kstack_hi = .;

    _end_maps = .;
    /* End mappings */

    /* Discard debugging, relocations, exceptions, etc */
    /DISCARD/ :
    {
	*(.stab*)
	*(.debug*)
	*(.comment*)
	*(.shstrtab*)
	*(.symtab*)
	*(.strtab*)
	*(.rel.*)
	*(.eh_frame)
    }
    /* I should see about exceptions... if they are implemented, I will
     * enable them
     */
}
