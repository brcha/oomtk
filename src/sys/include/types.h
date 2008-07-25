#ifndef __TYPES_H__
#define __TYPES_H__
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
 * @brief Type definitions
 */

#ifndef __ASSEMBLER__

#include <macros.h>
#include <config.h>
#include INC_ARCH(types.h)

/**
 * @brief Data type to store the CPU id
 */
#if MAX_NCPU <= 256
typedef uint8_t cpuid_t;
#else
typedef uint32_t cpuid_t;
#endif

/**
 * @brief Value to signal the allocation failure in the physical memory allocation.
 */
#define BAD_KPA (~((kpa_t) 0))

#ifndef NULL
#define NULL 0
#endif

#endif /* ! __ASSEMBLER__ */

#endif /* __TYPES_H__ */
