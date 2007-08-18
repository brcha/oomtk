#ifndef __IA32_CONFIG_H__
#define __IA32_CONFIG_H__
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
/** @file
 * @brief IA32 Configuration file
 */

/**
 * Console?
 */
#define HAVE_CONSOLE 1

/**
 * Number of physical regions
 */
#define PHYSMEM_NREGIONS 512

/**
 * Alignment for SMP -- should be more than CPU cache size
 */
#define CACHE_LINE_SIZE 128

/**
 * Number of pages in per-CPU stacks (2^n)
 */
#define KSTACK_NPAGES 0x1

/**
 * Kernel starting virtual address
 */
#define KVA 0xC0000000

/**
 * Smallest page size supported
 */
#define PAGE_SIZE       0x1000

/**
 * No of page offset bits (take a look at PAGE_SIZE and count bits :) )
 */
#define PAGE_ADDR_BITS  12

/**
 * Mask to extract page offset
 */
#define PAGE_MASK       0x0fff

/**
 * Number of virtual address bits
 */
#define ADDRESS_BITS    32

#endif /* __IA32_CONFIG_H__ */
