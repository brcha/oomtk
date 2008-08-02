#ifndef __OOMTKSYS_OPAGING_H__
#define __OOMTKSYS_OPAGING_H__
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
/** @file opaging.h
 * @brief Generic, platform independant paging class
 */
#include <types.h>
#include <fatal.h>
#include <ansi.h>

#define DEBUG_PAGING DEBUG_ENABLE

/**
 * @brief Generic, abstract, platform independent paging class
 */
class OPaging
{
  public:
    static OPaging * instance();
    
    /**
     * @brief Initialize the transient mappings
     */
    virtual void transientMappingsInitialize() = 0;
    
    /**
     * @brief Flush the released transient maps from the TLB
     */
    void transmapAdviseTlbFlush();
    
    /**
     * @brief Map the physical address using transmap
     * @param pa physical address
     * @returns the mapped virtual address
     */
    virtual kva_t map(kpa_t pa) = 0;
    
    /**
     * @brief Unmap the virtual address previously mapped using transmap
     * @param va the virtual address
     */
    virtual void unmap(kva_t va) = 0;

  protected:
    OPaging() {};
    virtual ~OPaging() {};

    uint32_t transmapPerCPUentry(uint32_t slot);
    kva_t    transmapEntryVA(uint32_t entry);
    uint32_t transmapVAentry(kva_t va);
    uint32_t transmapEntrySlot(uint32_t entry);
};

#endif /* __OOMTKSYS_OPAGING_H__ */
