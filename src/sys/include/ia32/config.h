#ifndef __IA32_CONFIG_H__
#define __IA32_CONFIG_H__
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@gna.org>
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
/** @file
 * @brief IA32 Configuration file
 */

#ifndef MAX_NCPU
/** @brief Maximum (static) number of CPUs supported */
#define MAX_NCPU		4
#endif

/* Transient mappings */
/** @brief Number of transient map entries per CPU */
#define TRANSMAP_ENTRIES_PER_CPU 64
/** @brief Number of CPUs supported by a single transmap page
 * 
 * NOTE: This assumes PAE. PTE can handle 1024, but these are safe values.
 */
#define TRANSMAP_CPUS_PER_PAGE	(512 / TRANSMAP_ENTRIES_PER_CPU)
/** @brief Number of transmap pages requires
 */
#define TRANSMAP_PAGES		((MAX_NCPU + TRANSMAP_CPUS_PER_PAGE - 1) / TRANSMAP_CPUS_PER_PAGE)

/** @brief Is there a console involved?
 */
#define HAVE_CONSOLE		1

/** @brief Number of entries in physical region list
 */
#define PHYSMEM_NREGIONS	512

/** @brief If 1 then using hierarchical mapping scheme.
 */
#define HIERARCHICAL_MAP	1

/** @brief The number of bits to index a PTE in a mapping table.
 * 
 * NOTE: This is only needed for hierarchical mapping.
 */
#define HIERARCHICAL_MAP_MAX_PTE_BITS 10

/** @brief Alignment value for cache aligned data structs
 */
#define CACHE_LINE_SIZE		128

/** @brief Number of top-level mapping tables per process
 */
#define MAPTABLES_PER_PROCESS	4

/** @brief Number of pages in per-CPU stacks (2^n)
 */
#define KSTACK_NPAGES		0x1

/** @brief Kernel starting virtual address
 */
#define KVA			0xC0000000

/** @brief Limit above which heap must not grow
 * 
 * NOTE: local APIC if at 0xff000000, so that is the limit
 */
#define HEAP_LIMIT_VA		0xFF000000

/** @brief Stack VA for CPU > 0
 */
#define SMP_STACK_VA		(HEAP_LIMIT_VA)

/** @brief Local APIC VA
 */
#define IA32_LOCAL_APIC_VA	(SMP_STACK_VA + (MAX_NCPU*KSTACK_NPAGES*OOMTK_PAGE_SIZE))
/** @brief IO-APIC VA
 */
#define IA32_IO_APIC_VA		(IA32_LOCAL_APIC_VA + 4096)

/** @brief VA of transient map (multiple of 4M)
 */
#define TRANSMAP_WINDOW_KVA	0xFF800000

/* Just check of PAE max address (0xFFFFFFFF) is not reached */
#if ((0xFFFFFFFF - (TRANSMAP_NPAGES * OOMTK_PAGE_SIZE)) + 1) < TRANSMAP_WINDOW_KVA
#error "TRANSMAP_WINDOW_KVA and HEAP_LIMIT_VA overlap => readjust for this no. of CPUs"
#endif

#endif /* __IA32_CONFIG_H__ */
