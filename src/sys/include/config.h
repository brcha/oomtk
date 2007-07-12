#ifndef __CONFIG_H__
#define __CONFIG_H__
/*
 *  Copyright (C) 1994-2006 by Filip Brcic <brcha@users.sourceforge.net>
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
 * @brief Configuration defines
 */

/* Take from arch-specific config.h first, then define what's left undefined here */
#include INC_ARCH(config.h)

/* Don't have console by default */
#ifndef HAVE_CONSOLE
#define HAVE_CONSOLE 0
#endif /* HAVE_CONSOLE */

#ifndef PHYSMEM_NREGIONS
/**
 * Number of physical memory regions
 */
#define PHYSMEM_NREGIONS 64
#endif /* PHYSMEM_NREGIONS */

#ifndef CACHE_LINE_SIZE
/**
 * Size of CPU Cache Line (CPU specific, so do define this in arch/__/config.h)
 */
#define CACHE_LINE_SIZE 64
#endif /* CACHE_LINE_SIZE */

#ifndef KSTACK_NPAGES
/**
 * Number of pages for per-CPU stacks (2^n)
 */
#define KSTACK_NPAGES 0x1
#endif /* KSTACK_NPAGES */

/**
 * Size of per-CPU kernel stack (in bytes)
 */
#define KSTACK_SIZE   (KSTACK_NPAGES * PAGE_SIZE)

/**
 * Mask to strip stack offset from stack pointer
 */
#define KSTACK_MASK   (~(KSTACK_NPAGES * PAGE_SIZE - 1))

#endif /* __CONFIG_H__ */
