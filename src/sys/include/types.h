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

#include <OOMTK_ARCH/types.h>

/**
 * @brief Data type to store addresses
 */
typedef void* addr_t;

#ifdef __cplusplus
/**
 * @brief Add offset to address
 * @param addr original address
 * @param off offset to add
 * @return new address
 */
INLINE addr_t addr_offset (addr_t addr, addr_t off)
{
    return (addr_t) ( (word_t) addr + (word_t) off);
}
#endif

/**
 * @brief Add offset to address
 * @param addr original address
 * @param off offset to add
 * @return new address
 */
INLINE addr_t addr_offset (addr_t addr, word_t off)
{
    return (addr_t) ( (word_t) addr + off);
}

/**
 * @brief Apply mask to an address
 * @param addr original address
 * @param mask address mask
 * @return new address
 */
INLINE addr_t addr_mask (addr_t addr, word_t mask)
{
    return (addr_t) ( (word_t) addr & mask);
}

/**
 * @brief Align address downwards.
 * It is assumed that the alignment is a power of 2.
 * @param addr original address
 * @param align alignment
 * @return new address
 */
INLINE addr_t addr_align (addr_t addr, word_t align)
{
    return addr_mask (addr, ~ (align - 1));
}

/**
 * @brief Align address upwards.
 * It is assumed that the alignment is a power of 2.
 * @param addr original address
 * @param align alignment
 * @return new address
 */
INLINE addr_t addr_align_up (addr_t addr, word_t align)
{
    return addr_mask (addr_offset (addr, align - 1), ~ (align - 1));
}

#ifndef NULL
#define NULL 0
#endif

#endif /* ! __ASSEMBLER__ */

#endif /* __TYPES_H__ */
