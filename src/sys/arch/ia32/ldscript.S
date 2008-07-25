/*
 *  Copyright (C) 2008 by Filip Brcic <brcha@gna.org>
 *
 *  This file is part of OOMTK
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
/** @file ldscript.S
 * @brief Linker script for ia32 kernel
 */

#include <ia32/pagesize.h>
#include <config.h>

ENTRY(_start)

OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH("i386")
TARGET("elf32-i386")

SECTIONS
{
	. = KVA + 0x00100000;	/* Kernel start at 1MB */
			
	/* Text (= code) */
	_text = .;
	.text : AT(ADDR(.text) - KVA)
	{
		*(.text)
	} = 0x9090
	_etext = .;
	
	_rodata = .;
	.rodata : AT(ADDR(.rodata) - KVA)
	{
		*(.rodata)
		*(.rodata.*)
	}
	_erodata = .;
	
	/* Syscalls */
	. = ALIGN(OOMTK_PAGE_SIZE);
	__syscall_page = .;
	.sc_trampoline : AT(ADDR(.sc_trampoline) - KVA)
	{
		*(.sc_trampoline)
		. = ALIGN(OOMTK_PAGE_SIZE);
	} = 0x9090
	_esyscall_page = .;
	
	. = ALIGN(OOMTK_PAGE_SIZE);
	__begin_maps = .;
	
	/* Kernel master page directory */
	KernPageDir = .;
	. = . + OOMTK_PAGE_SIZE;
	
	/* Kernel master page table for first 2M (PAE) or 4M (legacy) */
	. = ALIGN(OOMTK_PAGE_SIZE);
	KernPageTable = .;
	. = . + OOMTK_PAGE_SIZE;
	
	/* Page tables for transient mappings. */
	TransientMap = .;
	. += OOMTK_PAGE_SIZE * TRANSMAP_PAGES;
	__end_maps = .;
	
	/* Stack for cpu0 */
	. = ALIGN(KSTACK_SIZE);
	cpu0_kstack_lo = .;
	. = . + KSTACK_SIZE;
	cpu0_kstack_hi = .;
	
	/* Page-aligned data */
	. = ALIGN(OOMTK_PAGE_SIZE);
	_pagedata = .;
	.pagedata : AT(ADDR(.pagedata) - KVA)
	{
		*(.pagedata)
	}
	_epagedata = .;
	
	/* Normal data */
	. = ALIGN(OOMTK_PAGE_SIZE);
	_data = .;
	/* Cache aligned data */
	. = ALIGN(CACHE_LINE_SIZE);
	.data.cachealign : AT(ADDR(.data.cachealign) - KVA)
	{
		*(.data.cachealign)
		. = ALIGN(CACHE_LINE_SIZE);
	}
	.data : AT(ADDR(.data) - KVA)
	{
		*(.data)
	}
	_edata = .;
	
	/* Uninitialized data (BSS) */
	_bss_start = .;
	.bss : AT(ADDR(.bss) - KVA)
	{
		*(.bss)
	}
	_bss_end = .;
	
	.ctors ALIGN(4) : AT(ADDR(.ctors) - KVA)
	{
	  __ctors_start__ = .;
	  *(.ctors)
	  __ctors_end__ = .;
	  . = ALIGN(4);
	}
	
	.gnu.linkonce ALIGN(4) : AT(ADDR(.gnu.linkonce) - KVA)
	{
	  *(.gnu.linkonce.*)
	}
	    
	_end = .;
	
	/* C++ support (ctors/dtors) */
	/* TODO: to be included later */
	
	/* Debug support */
	.stab 0 :		{ *(.stab) }
	.stabstr 0 :		{ *(.stabstr) }
	.stab.excl 0 :		{ *(.stab.excl) }
	.stab.exclstr 0 :	{ *(.stab.exclstr) }
	.stab.index 0 :		{ *(.stab.index) }
	.stab.indexstr 0 :	{ *(.stab.indexstr) }
	.comment 0 :		{ *(.comment) }
	
	/* Discard C++ exceptions for now */
	/DISCARD/ :		{ *(.eh_frame) }
}