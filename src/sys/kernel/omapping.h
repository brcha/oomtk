#ifndef __OOMTKSYS_OMAPPING_H__
#define __OOMTKSYS_OMAPPING_H__
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
/** @file omapping.h
 * @brief Abstract address mapping class
 */

#include <OOMTK/OAgeList>

#include <types.h>
#include <cstddef>
#include <config.h>

/**
 * @brief Abstraction of virtual address mapping context
 * 
 * All architectures have some kind of memory mappings. This class defines
 * the abstract, platform-independant interface to such mappings. The mapping
 * is associated with every process (@see OProcess class). There is also a global
 * kernel mapping.
 */
class OMapping : public OAgeable
{
  public:
    OMapping();
    ~OMapping();
    
  protected:
    OMapping *    m_nextProduct;  ///< @brief Next mapping structure on the same product chain
//     MemHeader * producer;  ///< @brief Pointer to the object that produced this page table
    
    addr_t        m_match;        ///< @brief VA bits that must match for this table to be appropriate for use
    addr_t        m_mask;         ///< @brief VA bits that are significant for matching purposes
    uint8_t       m_restrictions; ///< @brief Permission restrictions that must be applied to all entries
    asid_t        m_asid;         ///< @brief Unique address space ID
    
#if MAPPING_INDEX_BITS > 0
    uint8_t       m_level;        ///< @brief Level of the page table
    size_t        m_userSlots;    ///< @brief Number of leading user-mode slots
    kpa_t         m_pa;           ///< @brief Physical address of the page table
#endif
};

#endif /* __OOMTKSYS_OMAPPING_H__ */
