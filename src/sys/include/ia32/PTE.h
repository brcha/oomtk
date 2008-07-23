#ifndef __IA32_PTE_H__
#define __IA32_PTE_H__
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
 * @brief Page Table flags
 */

/**
 * @defgroup PTE
 * @brief Page Table/Directory Entry flags
 *  (see Intel manuals for more info)
 * @{
 */
/// @brief Present (also known as Valid)
#define PTE_P     0x001
#define PTE_V     PTE_P
/// @brief Writable (R/W)
#define PTE_W     0x002
/// @brief User (User/Supervisor)
#define PTE_USER  0x004
/// @brief Write-through
#define PTE_PWT   0x008
/// @brief Cache-disable
#define PTE_PCD   0x010
/// @brief Accessed
#define PTE_A     0x020
/// @brief Dirty
#define PTE_D     0x040
/// @brief Page size (PDE, Pentium+ only)
#define PTE_PS    0x080
/// @brief Global page (PDE, PTE, Pentium Pro+ only)
#define PTE_GLBL  0x100
/// @brief Available for software use
#define PTE_SW0   0x200
/// @brief Available for software use
#define PTE_SW1   0x400
/// @brief Available for software use
#define PTE_SW2   0x800

/// @brief Mask to get PTE Base Address
#define PTE_FRAMEBITS   0xfffff000
/// @brief Mask to get info bits (permissions & stuff)
#define PTE_INFOBITS    0x00000fff
/** @} */

/**
 * @defgroup PAE
 * @brief Physical Address Extension
 * @{
 */
/// @brief Present (or valid)
#define PAE_P     0x001
#define PAE_V     PAE_P
/// @brief Writable
#define PAE_W     0x002
/// @brief User
#define PAE_USER  0x004
/// @brief Write-through
#define PAE_PWT   0x008
/// @brief Cache-disable
#define PAE_PCD   0x010
/// @brief Accessed
#define PAE_A     0x020
/// @brief Dirty
#define PAE_D     0x040
/// @brief Page Size
#define PAE_PS    0x080
/// @brief Global page
#define PAE_GLBL  0x100
/// @brief Available for software use
#define PAE_SW0   0x200
/// @brief Available for software use
#define PAE_SW1   0x400
/// @brief Available for software use
#define PAE_SW2   0x800

#endif /* __IA32_PTE_H__ */
