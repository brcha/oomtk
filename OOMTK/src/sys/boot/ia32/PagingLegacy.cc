/*
 *  Copyright (C) 2006-2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file
 * \brief Legacy Paging for IA-32
 */
/*
 * $Id$
 */

#include "PagingLegacy.h"

#include <stdio.h>
#include <ansi.h>
#include <CPUID.h>
#include INC_ARCH(CR.h)

/**
 * \brief Get the one instance of paging class
 * \returns the instance
 */
static PagingLegacy * PagingLegacy::instance()
{
  static PagingLegacy paging = PagingLegacy();

  return &paging;
};

/**
 * \defgroup Mappings
 * \brief Mappings as declared in linker.ia32.ld.S
 * @{
 */
extern void * MasterPageDir;  ///< \brief Master paging directory
extern void * KernPageTable;  ///< \brief Kernel paging table
extern void * TransMap;       ///< \brief Transient mappings page
/** @} */

/**
 * \brief Protected constructor
 */
PagingLegacy::PagingLegacy() :
    Paging()
{
  PageDirectory     = reinterpret_cast<PDE_t*>(&MasterPageDir);
  KernelPageTable   = reinterpret_cast<PTE_t*>(&KernPageTable);
  TransientMappings = reinterpret_cast<PTE_t*>(&TransMap);
};

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
virtual void PagingLegacy::setup()
{
  DEBUG_PAGING
      printf("Starting IA32 legacy paging ... ");
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

  DEBUG_PAGING
      printf(ANSI_FG_GREEN "[ok]\n" ANSI_NORMAL);
};
