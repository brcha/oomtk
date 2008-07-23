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
 * @defgroup PrintfFormats
 * @brief Printf formats for various types
 * @{
 */
#define OOMTK_PTR_FMT         "%08lx"
#define OOMTK_MWORD_FMT       "%08lx"
#define OOMTK_X64_FMT         "%016llx"
#define OOMTK_ADDR_INPUT_FMT  "%8x"
#define OOMTK_FRAME_INPUT_FMT "%5x"

#endif /* __ARCH__IA32__TYPES_H__ */
