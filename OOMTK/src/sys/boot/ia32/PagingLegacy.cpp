INTERFACE:
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** @file
 * @brief Legacy Paging for IA-32
 */

#include <Paging.h>
#include <types.h>

/**
 * @brief Legacy paging support for IA-32 processors
 *
 * This paging techinque requires PSE (page size extension) support (Pentium and newer).
 */
class PagingLegacy : public Paging
{
  protected:
    /**
     * @brief Page directory entry
     */
    union PageDirectoryEntry {
      struct {
        uint32_t  present     : 1;  ///< @brief Present
        uint32_t  rw          : 1;  ///< @brief Read/Write
        uint32_t  userSuper   : 1;  ///< @brief User/Supervisor
        uint32_t  pwt         : 1;  ///< @brief Page Write-Through
        uint32_t  pcd         : 1;  ///< @brief Page Cache Disabled
        uint32_t  accessed    : 1;  ///< @brief Accessed
        uint32_t  s           : 1;  ///< @brief Reserved (set to 0)
        uint32_t  ps          : 1;  ///< @brief Page size (0 indicates 4 kb)
        uint32_t  global      : 1;  ///< @brief Global page (ignored)
        uint32_t  avail       : 3;  ///< @brief Available for system programmer's use
        uint32_t  pt_base     :20;  ///< @brief Page-Table Base Address
      } bits4k;                     ///< @brief Page directory entry bits for 4 kb pages
      struct {
        uint32_t  present     : 1;  ///< @brief Present
        uint32_t  rw          : 1;  ///< @brief Read/Write
        uint32_t  userSuper   : 1;  ///< @brief User/Supervisor
        uint32_t  pwt         : 1;  ///< @brief Page Write-Through
        uint32_t  pcd         : 1;  ///< @brief Page Cache Disabled
        uint32_t  accessed    : 1;  ///< @brief Accessed
        uint32_t  dirty       : 1;  ///< @brief Dirty
        uint32_t  ps          : 1;  ///< @brief Page size (1 indicates 4 Mb)
        uint32_t  global      : 1;  ///< @brief Global page
        uint32_t  avail       : 3;  ///< @brief Available for system programmer's use
        uint32_t  pat         : 1;  ///< @brief Page attribute table index (set to 0 if not supported)
        uint32_t  reserved    : 9;  ///< @brief Reserved (must be set to 0) - AMD uses bits 20:13 for physical page base address bits 39:32, but since we are not going to address 1 TB address space and Intel doesn't support that, we will ignore that.
        uint32_t  page_base   :10;  ///< @brief Page Base Address
      } bits4m;                     ///< @brief Page directory entry bits for 4 Mb pages
      uint32_t    value;            ///< @brief Page directory entry 32bit value
    };

    /**
     * @brief Page Table Entry
     */
    union PageTableEntry {
      struct {
        uint32_t  present     : 1;  ///< @brief Present
        uint32_t  rw          : 1;  ///< @brief Read/Write
        uint32_t  userSuper   : 1;  ///< @brief User/Supervisor
        uint32_t  pwt         : 1;  ///< @brief Page Write-Through
        uint32_t  pcd         : 1;  ///< @brief Page Cache Disabled
        uint32_t  accessed    : 1;  ///< @brief Accessed
        uint32_t  dirty       : 1;  ///< @brief Dirty
        uint32_t  pat         : 1;  ///< @brief Page attribute table index (set to 0 if not supported)
        uint32_t  global      : 1;  ///< @brief Global page
        uint32_t  avail       : 3;  ///< @brief Available for system programmer's use
        uint32_t  page_base   :20;  ///< @brief Page Base Address
      } bits4k;
      uint32_t    value;            ///< @brief Page table entry 32bit value
    };

    PageDirectoryEntry * PageDirectory;
    PageTableEntry * KernelPageTable;
    PageTableEntry * TransientMappings;
};

IMPLEMENTATION:

#include <stdio.h>
#include <CPUID.h>
#include INC_ARCH(CR.h)

/**
 * @brief Get the one instance of paging class
 * @returns the instance
 */
PUBLIC static PagingLegacy * PagingLegacy::instance()
{
  static PagingLegacy paging = PagingLegacy();

  return &paging;
};

/**
 * @defgroup Mappings
 * @brief Mappings as declared in linker.ia32.ld.S
 * @{
 */
extern void * MasterPageDir;  ///< @brief Master paging directory
extern void * KernPageTable;  ///< @brief Kernel paging table
extern void * TransMap;       ///< @brief Transient mappings page
/** @} */

/**
 * @brief Protected constructor
 */
PROTECTED PagingLegacy::PagingLegacy()
{
  PageDirectory = reinterpret_cast<PageDirectoryEntry*>(&MasterPageDir);
  KernelPageTable = reinterpret_cast<PageTableEntry*>(&KernPageTable);
  TransientMappings = reinterpret_cast<PageTableEntry*>(&TransMap);
};

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
PUBLIC virtual void PagingLegacy::setup()
{
  // Map first [0,4M]
  uint32_t address = 0u;
  for (size_t i = 0; i < 1024; i++)
  {
    // set the bits
    KernelPageTable[i].value            = address;
    KernelPageTable[i].bits4k.present   = 1;
    KernelPageTable[i].bits4k.rw        = 1;
    KernelPageTable[i].bits4k.accessed  = 1;
    KernelPageTable[i].bits4k.dirty     = 1;
    // update the address for the next run
    address += 0x1000; // = 4 kb
  }

  // Set the paging directory to point to kernel page table

  // First set the first 0-4 Mb
  PageDirectory[0].value              =
      reinterpret_cast<uint32_t>(&KernelPageTable[0]) - KVA;
  PageDirectory[0].bits4k.present     = 1;
  PageDirectory[0].bits4k.rw          = 1;
  PageDirectory[0].bits4k.accessed    = 1;
  // 4 kb page - not needed, but this clarifies syntax
  PageDirectory[0].bits4k.ps          = 0;

  // Next, set the KVA - KVA + 4 Mb to point to the same place
  // NOTE: Index of PDE is KVA >> 22
  // if KVA is 0xC0000000 = 3 GB, then index is 0x00000300 = 768
  //PageDirectory[KVA >> 22].value = 0u; <- no need for this, all is cleared in start.S
  PageDirectory[KVA >> 22].value              =
      reinterpret_cast<uint32_t>(&KernelPageTable[0]) - KVA;
  PageDirectory[KVA >> 22].bits4k.present     = 1;
  PageDirectory[KVA >> 22].bits4k.rw          = 1;
  PageDirectory[KVA >> 22].bits4k.accessed    = 1;
  PageDirectory[KVA >> 22].bits4k.ps          = 0;

  // Put the Transient mappings to the top 12 Mb
  // 0x100000000 - 0x00C00000 (12Mb) = 0xFF400000
  // 0xFF400000 >> 22 = 0xFF4
  PageDirectory[0xFF4].value                  =
      reinterpret_cast<uint32_t>(&TransientMappings[0]) - KVA;
  PageDirectory[0xFF4].bits4k.present         = 1;
  PageDirectory[0xFF4].bits4k.rw              = 1;

  // Go and set things up
  uint32_t CR0, CR4;

  // First set the flags in CR4
  CR4 = cr4();

  CPUID * cpuid = CPUID::instance();

  cpuid->identify(); // identify CPU if not already identified
  if (cpuid->has(CPUID::PSE))
    CR4 |= CR4_PSE;
  if (cpuid->has(CPUID::PGE))
    CR4 |= CR4_PGE;

  // Store the new value of CR4
  cr4(CR4);

  // Put the physical address of PageDirectory into CR3
  cr3(reinterpret_cast<uint32_t>(&PageDirectory[0]) - KVA);

  // At last, enable paging
  CR0 = cr0();

  CR0 |= CR0_PG; // paging
  CR0 |= CR0_AM; // alignment mask
  CR0 |= CR0_WP; // write protect
  CR0 |= CR0_ET; // extension type
  CR0 |= CR0_TS; // task switched
  CR0 |= CR0_MP; // monitor coprocessor
  CR0 |= CR0_PE; // protection enable (this should have been enabled by grub)

  cr0(CR0);
};
