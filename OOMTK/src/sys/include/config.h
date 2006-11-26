#ifndef __CONFIG_H__
#define __CONFIG_H__
/*
 *  Copyright (C) 1994-2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK v.2
 */
/*
 * $Id: config.h 181 2006-10-12 22:28:03Z brcha $
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
