#ifndef __IA32_OPAGINGLEGACY_H__
#define __IA32_OPAGINGLEGACY_H__
/*
 *  Copyright (C) 2006-2008 by Filip Brcic <brcha@gna.org>
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
/** @file
 * @brief Legacy Paging for IA-32
 */
#include "opaging-ia32.h"
#include <types.h>

/* Several macros for convenience */
#define NPTE_PER_PAGE         (OOMTK_PAGE_SIZE/sizeof(OPagingLegacy::PTE_t))
#define PTE_PFRAME_BOUND      (((kpa_t) 1) << 20)
#define PTE_PADDR_BOUND       (((kpa_t) 1) << 32)
#define PTE_PGDIR_NDX(va)     ((va) >> 22)
#define PTE_PGTBL_NDX(va)     (((va) >> 12) % NPTE_PER_PAGE)
// kpa_t is 64bit, and all pa are 32bit, so downcast vefore shifting...
#define PTE_KPA_TO_FRAME(pa)  (((uint32_t)(pa)) >> 12)
#define PTE_FRAME_TO_KPA(frm) ((frm) << 12)
#define PTE_CLEAR(pte)        (pte).value = 0

/**
 * @brief Legacy paging support for IA-32 processors
 *
 * This paging techinque requires PSE (page size extension) support (Pentium and newer).
 */
class OPagingLegacy : public OPagingIA32
{
  public:
    /**
     * @brief Get the one instance of paging class
     * @returns the instance
     */
    static OPagingLegacy * instance();

    /**
     * @brief Setup paging
     *
     * After booting, the kernel is located at physical memory location 0x00100000,
     * but kernel is relocated at 0xC0100000 (= KVA + 0x00100000 = 3 GB + 1 MB).
     *
     * This method should map the physical memory [0, 4M] to virtual [0, 4M] and also
     * to [KVA, KVA + 4M].
     *
     * PageDirectory has 1024 PDEs that map 1024*4M = 4G memory
     */
    virtual void setup();
    
    /**
     * @brief Initialize transient mappings
     */
    virtual void transientMappingsInitialize();

    /**
     * @brief Map the physical address using transmap
     * @param pa physical address
     * @returns the mapped virtual address
     */
    virtual kva_t map(kpa_t pa);
    
    /**
     * @brief Unmap the virtual address previously mapped using transmap
     * @param va the virtual address
     */
    virtual void unmap(kva_t va);
    
  protected:
    /**
     * @brief Protected constructor
     */
    OPagingLegacy();

  protected:
    /**
     * @brief Page directory entry
     */
    union PDE_t
    {
      struct
      {
        uint32_t  present     : 1;  ///< @brief Present
        uint32_t  rw          : 1;  ///< @brief Read/Write
        uint32_t  userSuper   : 1;  ///< @brief User/Supervisor
        uint32_t  pwt         : 1;  ///< @brief Page Write-Through
        uint32_t  pcd         : 1;  ///< @brief Page Cache Disabled
        uint32_t  accessed    : 1;  ///< @brief Accessed
        uint32_t  s           : 1;  ///< @brief Reserved (set to 0)
        uint32_t  ps          : 1;  ///< @brief Page size (0 indicates 4 kb)
        uint32_t  global      : 1;  ///< @brief Global page (ignored)
        uint32_t  available   : 3;  ///< @brief Available for system programmer's use
        uint32_t  pt_base     : 20; ///< @brief Page-Table Base Address
      } bits4k;                     ///< @brief Page directory entry bits for 4 kb pages

      struct
      {
        uint32_t  present     : 1;  ///< @brief Present
        uint32_t  rw          : 1;  ///< @brief Read/Write
        uint32_t  userSuper   : 1;  ///< @brief User/Supervisor
        uint32_t  pwt         : 1;  ///< @brief Page Write-Through
        uint32_t  pcd         : 1;  ///< @brief Page Cache Disabled
        uint32_t  accessed    : 1;  ///< @brief Accessed
        uint32_t  dirty       : 1;  ///< @brief Dirty
        uint32_t  ps          : 1;  ///< @brief Page size (1 indicates 4 Mb)
        uint32_t  global      : 1;  ///< @brief Global page
        uint32_t  available   : 3;  ///< @brief Available for system programmer's use
        uint32_t  pat         : 1;  ///< @brief Page attribute table index (set to 0 if not supported)
        uint32_t  reserved    : 9;  ///< @brief Reserved (must be set to 0) - AMD uses bits 20:13 for physical page base address bits 39:32, but since we are not going to address 1 TB address space and Intel doesn't support that, we will ignore that.
        uint32_t  page_base   : 10; ///< @brief Page Base Address
      } bits4m;                     ///< @brief Page directory entry bits for 4 Mb pages
      uint32_t    value;            ///< @brief Page directory entry 32bit value
    };

    /**
     * @brief Page Table Entry
     */
    union PTE_t
    {

      struct
      {
        uint32_t  present     : 1;  ///< @brief Present
        uint32_t  rw          : 1;  ///< @brief Read/Write
        uint32_t  userSuper   : 1;  ///< @brief User/Supervisor
        uint32_t  pwt         : 1;  ///< @brief Page Write-Through
        uint32_t  pcd         : 1;  ///< @brief Page Cache Disabled
        uint32_t  accessed    : 1;  ///< @brief Accessed
        uint32_t  dirty       : 1;  ///< @brief Dirty
        uint32_t  pat         : 1;  ///< @brief Page attribute table index (set to 0 if not supported)
        uint32_t  global      : 1;  ///< @brief Global page
        uint32_t  available   : 3;  ///< @brief Available for system programmer's use
        uint32_t  page_base   : 20; ///< @brief Page Base Address
      } bits4k;
      uint32_t    value;            ///< @brief Page table entry 32bit value
    };

    /**
     * @brief Master page directory
     */
    PDE_t * m_PageDirectory;

    /**
     * @brief Kernel's page table
     */
    PTE_t * m_KernelPageTable;

    /**
     * @brief Transient mappings
     *
     * Idea is to remap the page from user-space to kernel-space using this page
     * table.
     */
    PTE_t * m_TransientMappings;
};


#endif /* __PagingLegacy_H__ */
