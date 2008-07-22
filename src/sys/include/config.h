#ifndef __CONFIG_H__
#define __CONFIG_H__
/*
 *  Copyright (C) 2006-2008 by Filip Brcic <brcha@gna.org>
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
 * @brief Configuration definitions
 */

/* Take from arch-specific config.h first, then define what's left undefined here */
#include <OOMTK_ARCH/config.h>

#ifndef MAX_NCPU
/** @brief Maximum number of CPUs
 */
#define MAX_NCPU	1
#endif

#ifndef HAVE_HUI
/** @brief Human UI requires console i/o to be compiled.
 */
#define HAVE_HUI	1
#endif

/* Don't have console by default */
#ifndef HAVE_CONSOLE
/** @brief Is there a console?
 */
#define HAVE_CONSOLE 0
#endif

#ifndef HIERARCHICAL_MAP
/** @brief If 1 then using hierarchical mapping scheme
 */
#define HIERARCHICAL_MAP	0
#endif

#ifndef PHYSMEM_NREGIONS
/** @brief Number of physical memory regions
 */
#define PHYSMEM_NREGIONS	64
#endif

#ifndef CACHE_LINE_SIZE
/** @brief Approximate size of CPU cache line
 */
#define CACHE_LINE_SIZE		64
#endif

#ifndef KSTACK_NPAGES
/** @brief Number of pages for per-CPU stacks (2^n)
 */
#define KSTACK_NPAGES		0x1
#endif

#ifndef MAPTABLES_PER_PROCESS
/** @brief Number of top-level mapping pages per process.
 */
#define MAPTABLES_PER_PROCESS	0x1
#endif

/** @brief Size of per-CPU kernel stack (in bytes)
 */
#define KSTACK_SIZE		(KSTACK_NPAGES * OOMTK_PAGE_SIZE)

/** @brief Mask to strip stack offset from stack pointer
 */
#define KSTACK_MASK		(~(KSTACK_NPAGES * OOMTK_PAGE_SIZE - 1))

#endif /* __CONFIG_H__ */
