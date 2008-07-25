#ifndef __IA32_PAGESIZE_H__
#define __IA32_PAGESIZE_H__

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
/** @file pagesize.h
 * @brief Arch-specific page size definitions
 * 
 * NOTE: This file is included by assembly code, so incomatible constructs should be avoided.
 */

/** @brief Smallest page size supported by the kernel.
 * 
 * NOTE: by the kernel, so can be larger than the smallest page size supported by the architecture
 */
#define OOMTK_IA32_PAGE_SIZE		0x1000

/** @brief Number of page offset bits
 */
#define OOMTK_IA32_PAGE_ADDR_BITS	12

/** @brief Mask value for the page offset bits
 */
#define OOMTK_IA32_PAGE_ADDR_MASK	0xfff

/** @brief Number of VA bits
 */
#define OOMTK_IA32_ADDRESS_BITS		32

#ifdef OOMTK_ARCH_IA32
// just a precaution, in case somebody needs this file on non-ia32 arch
 
#define OOMTK_PAGE_SIZE			OOMTK_IA32_PAGE_SIZE
#define OOMTK_PAGE_ADDR_BITS		OOMTK_IA32_PAGE_ADDR_BITS
#define OOMTK_PAGE_ADDR_MASK		OOMTK_IA32_PAGE_ADDR_MASK
#define OOMTK_HW_ADDRESS_BITS		OOMTK_IA32_ADDRESS_BITS

#endif /* OOMTK_ARCH_IA32 */

#endif /* __IA32_PAGESIZE_H__ */
