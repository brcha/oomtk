#ifndef __libglue_VPSTRING_H__
#define __libglue_VPSTRING_H__
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
/** @file vpstring.h
 * @brief Methods for manipulating virtual-to-physical and vice versa copying
 * 
 * These methods originate from string.h, and hense the name vpstring.h
 */

#include <macros.h>
#include <types.h>
#include <stddef.h>

__BEGIN_DECLS

/**
 * @brief Copy memory area from virtual to physical address
 * @param dest_pa destination physical address
 * @param source_va source virtual address (pointer)
 * @param length length of the buffer
 */
void memcpy_vtop(kpa_t  dest_pa, void * source_va, size_t length);
/**
 * @brief Copy memory area from physical to virtual address
 * @param dest_va destination virtual address (pointer)
 * @param source_pa source physical address
 * @param length length of the buffer
 */
void memcpy_ptov(void * dest_va, kpa_t  source_pa, size_t length);
/**
 * @brief Copy memory area from physical to physical address
 * @param dest_pa destination physical address
 * @param source_pa source physical address
 * @param length length of the buffer
 */
void memcpy_ptop(kpa_t  dest_pa, kpa_t  source_pa, size_t length);
/**
 * @brief Set memory area at the physical address
 * @param pa physical address of the memory area
 * @param value value to be set
 * @param length length of the buffer
 */
void memset_p   (kpa_t  pa,      int    value,     size_t length);

__END_DECLS

#endif /* __libglue_VPSTRING_H__ */
