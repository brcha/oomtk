#ifndef __IA32_OPAGINGPAE_H__
#define __IA32_OPAGINGPAE_H__
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
 * @brief Paging for PAE enabled processors
 */
#include "opaging.h"

// A few PAE macros
#define NPAE_PER_PAGE             (OOMTK_PAGE_SIZE/sizeof(OPagingPAE::PTE_t))
#define PAE_PFRAME_BOUND          (((kpa_t) 1) << 24)
#define PAE_PADDR_BOUND           (((kpa_t) 1) << 36)
#define PAE_PDPT_NDX(va)          (((va) >> 30) % 4)
#define PAE_PGDIR_NDX(va)         (((va) >> 21) % NPAE_PER_PAGE)
#define PAE_PGTBL_NDX(va)         (((va) >> 12) % NPAE_PER_PAGE)
#define PAE_KPA_TO_FRAME(pa)      ((pa) >> 12)
#define PAE_FRAME_TO_KPA(frm)     ((frm) << 12)
#define PAE_CLEAR(pae)            (pae).value = 0

#define KPA_IS_PAGE_ADDRESS(pa)   ((pa & ((kpa_t)0xfff)) == 0)

/**
 * @brief PAE Paging support for IA32
 */

class OPagingPAE : public OPaging
{
  public:
    /**
     * @brief Get the one instance of paging class
     * @returns the instance
     */
    static OPagingPAE * instance();

    /**
     * @brief Setup paging
     *
     * After booting, the kernel is located at physical memory location 0x00100000,
     * but kernel is relocated at 0xC0100000 (= KVA + 0x00100000 = 3 GB + 1 MB).
     *
     * This method should map the physical memory [0, 2M] to virtual [0, 2M] and also
     * to [KVA, KVA + 2M].
     *
     * PT and PD have 512 entries.
     */
    virtual void setup();
    
    /**
     * @brief Initialize the transient mappings
     */
    virtual void transientMappingsInitialize();

  protected:
    /**
     * @brief Protected constructor
     */
    OPagingPAE();

  protected:
    /**
     * @brief PDPT register format (= cr3)
     */
    union PDPT_Register_t
    {
      struct
      {
        uint32_t  mbz         : 3;  ///< @brief Must be zero
        uint32_t  pwt         : 1;  ///< @brief Page write-through
        uint32_t  pcd         : 1;  ///< @brief Page cache disable
        uint32_t  pdpt_base   : 27; ///< @brief Base address of PDPT
      } bits;                       ///< @brief Individual bits of PDPT register
      uint32_t    value;            ///< @brief 32bit value of PDPT register
    };

    /**
     * @brief Page-Directory-Pointer-Table entry
     */
    union PDPTE_t
    {
      struct
      {
        uint64_t  present     : 1;  ///< @brief Present
        uint64_t  mbz1        : 2;  ///< @brief Reserved and must be zero
        uint64_t  pwt         : 1;  ///< @brief Page write-through
        uint64_t  pcd         : 1;  ///< @brief Page cache disable
        uint64_t  mbz2        : 4;  ///< @brief Reserved and must be zero
        uint64_t  available   : 3;  ///< @brief Available for system programmer's use
        uint64_t  pd_base     : 24; ///< @brief Page directory base address
                // note: AMD supports 40 bits for pd_base, but Intel doesn't, so :(
        uint64_t  mbz3        : 28; ///< @brief Reserved and must be zero
      } bits;                       ///< @brief Bits of PDPTE
      uint64_t    value;            ///< @brief 64bit value of PDPT entry
    };

    /**
     * @brief Page directory entry
     */
    union PDE_t
    {
      struct
      {
        uint64_t  present     : 1;  ///< @brief Present
        uint64_t  rw          : 1;  ///< @brief Read/Write
        uint64_t  userSuper   : 1;  ///< @brief User/Supervisor
        uint64_t  pwt         : 1;  ///< @brief Page write-through
        uint64_t  pcd         : 1;  ///< @brief Page cache disable
        uint64_t  accessed    : 1;  ///< @brief Accessed
        uint64_t  mbz1        : 1;  ///< @brief Reserved (must be zero)
        uint64_t  ps          : 1;  ///< @brief Page size (0 = 4k)
        uint64_t  mbz2        : 1;  ///< @brief Reserved (must be zero)
        uint64_t  available   : 3;  ///< @brief Available for system programmer's use
        uint64_t  pt_base     : 24; ///< @brief Page table base address
                // note: AMD supports 40 bits for pt base address!
        uint64_t  mbz3        : 27; ///< @brief Reserved (must be zero)
        uint64_t  nx          : 1;  ///< @brief No-eXecute bit (AMD's NX & Intel's XD)
      } bits4k;                     ///< @brief PDE bits for 4k pages

      struct
      {
        uint64_t  present     : 1;  ///< @brief Present
        uint64_t  rw          : 1;  ///< @brief Read/Write
        uint64_t  userSuper   : 1;  ///< @brief User/Supervisor
        uint64_t  pwt         : 1;  ///< @brief Page write-through
        uint64_t  pcd         : 1;  ///< @brief Page cache disable
        uint64_t  accessed    : 1;  ///< @brief Accessed
        uint64_t  dirty       : 1;  ///< @brief Dirty
        uint64_t  ps          : 1;  ///< @brief Page size (1 = 2m)
        uint64_t  global      : 1;  ///< @brief Global
        uint64_t  available   : 3;  ///< @brief Available for system programmer's use
        uint64_t  pat         : 1;  ///< @brief Page attribute table index
        uint64_t  mbz1        : 8;  ///< @brief Reserved and must be zero
        uint64_t  page_base   : 15; ///< @brief Physical page base address
                // note: AMD supports 31 bits for page_base, but Intel doesn't, so :(
        uint64_t  mbz2        : 27; ///< @brief Reserved and must be zero
        uint64_t  nx          : 1;  ///< @brief No eXecute bit (AMD's NX, Intel's XD)
      } bits2m;                     ///< @brief PDE bits for 2m pages
      uint64_t    value;            ///< @brief 64bit value of PDE
    };

    /**
     * @brief Page Table Entry
     */
    union PTE_t
    {
      struct
      {
        uint64_t  present     : 1;  ///< @brief Present
        uint64_t  rw          : 1;  ///< @brief Read/Write
        uint64_t  userSuper   : 1;  ///< @brief User/Supervisor
        uint64_t  pwt         : 1;  ///< @brief Page write-through
        uint64_t  pcd         : 1;  ///< @brief Page cache disable
        uint64_t  accessed    : 1;  ///< @brief Accessed
        uint64_t  dirty       : 1;  ///< @brief Dirty
        uint64_t  pat         : 1;  ///< @brief Page Attribute Table index
        uint64_t  global      : 1;  ///< @brief Global page
        uint64_t  available   : 3;  ///< @brief Available for system programmer's use
        uint64_t  page_base   : 24; ///< @brief Physical page base address
                // note: AMD again supports 40 bits for page_base
        uint64_t  mbz         : 27; ///< @brief Reserved (must be zero)
        uint64_t  nx          : 1;  ///< @brief No eXecute (AMD's NX and Intel's XD)
      } bits4k;
      uint64_t    value;            ///< @brief 64bit value of PTE
    };

    /**
     * @brief Kernel's page directory pointer table
     *
     * This PDPT consists of 4 PDPTE's and this is the highest level paging
     * structure while PAE is enabled.
     */
    PDPTE_t * m_KernelPDPT;

    /**
     * @brief Master page directory
     */
    PDE_t   * m_PageDirectory;

    /**
     * @brief Kernel's page table
     */
    PTE_t   * m_KernelPageTable;

    /**
     * @brief Transient mappings
     *
     * Idea is to remap the page from user-space to kernel-space using this page
     * table.
     */
    PTE_t   * m_TransientMappings;
};


#endif /* __PagingPAE_H__ */
