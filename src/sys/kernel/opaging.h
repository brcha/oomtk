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
#include <cstddef>

#define DEBUG_PAGING DEBUG_DISABLE

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
    
    enum {
      // Permissions for kmap
      KMAP_R      = 0x1u,
      KMAP_W      = 0x2u,
      KMAP_X      = 0x4u,
      KMAP_NC     = 0x8u,
    };
    
    /**
     * @brief Map the area to the kernel master map
     * @param va target virtual address
     * @param pa physical address of the start of the area (must be aligned at page boundary)
     * @param size size of the memory area (must be multiple of OOMTK_PAGE_SIZE)
     * @param perms permissions
     * @param remap replace any pre-existing mappings if true
     * 
     * @warning This method is not SMP-safe. After initialization of APs, this function can be
     * used only for CPU-local mappings.
     */
    virtual void kmap(kva_t va, kpa_t pa, size_t size, uint32_t perms, bool remap) = 0;

  protected:
    OPaging() {};
    virtual ~OPaging() {};

    uint32_t transmapPerCPUentry(uint32_t slot);
    kva_t    transmapEntryVA(uint32_t entry);
    uint32_t transmapVAentry(kva_t va);
    uint32_t transmapEntrySlot(uint32_t entry);
};

#endif /* __OOMTKSYS_OPAGING_H__ */
