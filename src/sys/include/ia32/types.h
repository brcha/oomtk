#ifndef __ARCH__IA32__TYPES_H__
#define __ARCH__IA32__TYPES_H__
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
 * @brief Basic types for IA32
 */

typedef unsigned int __attribute__((__mode__(__DI__))) uint64_t;
// typedef unsigned long long int  uint64_t;
typedef unsigned int            uint32_t;
typedef unsigned short          uint16_t;
typedef unsigned char           uint8_t;

typedef signed int __attribute__((__mode__(__DI__))) int64_t;
// typedef signed long long int    int64_t;
typedef signed int              int32_t;
typedef signed short            int16_t;
typedef signed char             int8_t;

/**
 * @brief Machine word wide unsigned int
 */
typedef uint32_t                word_t;

/// @brief How many bits does machine word have?
enum {
  WORD_BITS = 32
};

/**
 * @brief Type sufficient to hold a kernel-implemented physical address.
 */
typedef uint64_t kpa_t;
typedef uint64_t addr_t;

/**
 * @brief Type sufficient to hold a count of items that will fit in the physical memory.
 */
typedef uint64_t kpsize_t;

/**
 * @brief Type sufficient to hold a kernel virtual address.
 */
typedef uint32_t kva_t;

/**
 * @brief Type sufficient to hold a user virtual address.
 */
typedef uint32_t uva_t;

/**
 * @brief Type sufficient to hold a transmap meta-map entries
 */
#if TRANSMAP_ENTRIES_PER_CPU <= 32
typedef uint32_t transmeta_t;
#else
typedef uint64_t transmeta_t;
#endif

/**
 * @brief Type of a target address space ID.
 */
typedef uint32_t asid_t;

/**
 * @brief Type of an object ID
 */
typedef uint64_t oid_t;

/**
 * @defgroup PrintfFormats
 * @brief Printf formats for various types
 * @{
 */
#define OOMTK_PTR_FMT         "%08lx"
#define OOMTK_MWORD_FMT       "%08lx"
#define OOMTK_X64_FMT         "%016llx"
#define OOMTK_ADDR_INPUT_FMT  "%8x"
#define OOMTK_FRAME_INPUT_FMT "%5x"

#define OOMTK_KPA_FMT         OOMTK_X64_FMT
#define OOMTK_KVA_FMT         OOMTK_PTR_FMT
#define OOMTK_UVA_FMT         OOMTK_PTR_FMT
/** @} */

#endif /* __ARCH__IA32__TYPES_H__ */
