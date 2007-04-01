#ifndef __PagingLegacy_H__
#define __PagingLegacy_H__
/*
 *  Copyright (C) 2006-2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file PagingLegacy.h
 * \brief Legacy Paging for IA-32
 */
/*
 * $Id$
 */
#include <Paging.h>
#include <types.h>

/**
 * \brief Legacy paging support for IA-32 processors
 *
 * This paging techinque requires PSE (page size extension) support (Pentium and newer).
 */
class PagingLegacy : public Paging
{
  public:
    /**
     * \brief Get the one instance of paging class
     * \returns the instance
     */
    static PagingLegacy * instance();

    /**
     * \brief Setup paging
     *
     * After booting, the kernel is located at physical memory location 0x00100000,
     * but kernel is relocated at 0xC0100000 (= KVA + 0x00100000 = 3 GB + 1 MB).
     *
     * This method should map the physical memory [0, 4M] to virtual [0, 4M] and also
     * to [KVA, KVA + 4M].
     *
     * PageDirectory has 1024 PDEs that map 1024*4M = 4G memory
     */
    virtual void PagingLegacy::setup();

  protected:
    /**
     * \brief Protected constructor
     */
    PagingLegacy();

  protected:
    /**
     * \brief Page directory entry
     */
    union PDE_t
    {
      struct
      {
        uint32_t  present     : 1;  ///< \brief Present
        uint32_t  rw          : 1;  ///< \brief Read/Write
        uint32_t  userSuper   : 1;  ///< \brief User/Supervisor
        uint32_t  pwt         : 1;  ///< \brief Page Write-Through
        uint32_t  pcd         : 1;  ///< \brief Page Cache Disabled
        uint32_t  accessed    : 1;  ///< \brief Accessed
        uint32_t  s           : 1;  ///< \brief Reserved (set to 0)
        uint32_t  ps          : 1;  ///< \brief Page size (0 indicates 4 kb)
        uint32_t  global      : 1;  ///< \brief Global page (ignored)
        uint32_t  available   : 3;  ///< \brief Available for system programmer's use
        uint32_t  pt_base     : 20; ///< \brief Page-Table Base Address
      } bits4k;                     ///< \brief Page directory entry bits for 4 kb pages

      struct
      {
        uint32_t  present     : 1;  ///< \brief Present
        uint32_t  rw          : 1;  ///< \brief Read/Write
        uint32_t  userSuper   : 1;  ///< \brief User/Supervisor
        uint32_t  pwt         : 1;  ///< \brief Page Write-Through
        uint32_t  pcd         : 1;  ///< \brief Page Cache Disabled
        uint32_t  accessed    : 1;  ///< \brief Accessed
        uint32_t  dirty       : 1;  ///< \brief Dirty
        uint32_t  ps          : 1;  ///< \brief Page size (1 indicates 4 Mb)
        uint32_t  global      : 1;  ///< \brief Global page
        uint32_t  available   : 3;  ///< \brief Available for system programmer's use
        uint32_t  pat         : 1;  ///< \brief Page attribute table index (set to 0 if not supported)
        uint32_t  reserved    : 9;  ///< \brief Reserved (must be set to 0) - AMD uses bits 20:13 for physical page base address bits 39:32, but since we are not going to address 1 TB address space and Intel doesn't support that, we will ignore that.
        uint32_t  page_base   : 10; ///< \brief Page Base Address
      } bits4m;                     ///< \brief Page directory entry bits for 4 Mb pages
      uint32_t    value;            ///< \brief Page directory entry 32bit value
    };

    /**
     * \brief Page Table Entry
     */
    union PTE_t
    {

      struct
      {
        uint32_t  present     : 1;  ///< \brief Present
        uint32_t  rw          : 1;  ///< \brief Read/Write
        uint32_t  userSuper   : 1;  ///< \brief User/Supervisor
        uint32_t  pwt         : 1;  ///< \brief Page Write-Through
        uint32_t  pcd         : 1;  ///< \brief Page Cache Disabled
        uint32_t  accessed    : 1;  ///< \brief Accessed
        uint32_t  dirty       : 1;  ///< \brief Dirty
        uint32_t  pat         : 1;  ///< \brief Page attribute table index (set to 0 if not supported)
        uint32_t  global      : 1;  ///< \brief Global page
        uint32_t  available   : 3;  ///< \brief Available for system programmer's use
        uint32_t  page_base   : 20; ///< \brief Page Base Address
      } bits4k;
      uint32_t    value;            ///< \brief Page table entry 32bit value
    };

    /**
     * \brief Master page directory
     */
    PDE_t * PageDirectory;

    /**
     * \brief Kernel's page table
     */
    PTE_t * KernelPageTable;

    /**
     * \brief Transient mappings
     *
     * Idea is to remap the page from user-space to kernel-space using this page
     * table.
     */
    PTE_t * TransientMappings;
};


#endif /* __PagingLegacy_H__ */
