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

#include "opaginglegacy.h"

#include <stdio.h>
#include <ansi.h>
#include <string.h>

#include <config.h>

#include <ia32/pagesize.h>
#include <ia32/CR.h>

/**
 * @brief Get the one instance of paging class
 * \returns the instance
 */
OPagingLegacy * OPagingLegacy::instance()
{
  static OPagingLegacy paging = OPagingLegacy();

  return &paging;
};

/**
 * \defgroup Mappings
 * @brief Mappings as declared in ldscript.S
 * @{
 */
extern kva_t KernPageDir[];    ///< @brief Master paging directory
extern kva_t KernPageTable[];  ///< @brief Kernel paging table
extern kva_t TransientMap[];   ///< @brief Transient mappings page
/** @} */

/**
 * @brief Protected constructor
 */
OPagingLegacy::OPagingLegacy() :
    OPaging()
{
  m_PageDirectory     = reinterpret_cast<PDE_t*>(&KernPageDir[0]);
  m_KernelPageTable   = reinterpret_cast<PTE_t*>(&KernPageTable[0]);
  m_TransientMappings = reinterpret_cast<PTE_t*>(&TransientMap[0]);
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
void OPagingLegacy::setup()
{
/*  DEBUG_PAGING
      printf("Starting IA32 legacy paging ... ");
  // Map first [0,4M]
  uint32_t address = 0u;
  for (size_t i = 0; i < 1024; i++)
  {
    // set the bits
    m_KernelPageTable[i].value            = address;
    m_KernelPageTable[i].bits4k.present   = 1;
    m_KernelPageTable[i].bits4k.rw        = 1;
    m_KernelPageTable[i].bits4k.accessed  = 1;
    m_KernelPageTable[i].bits4k.dirty     = 1;
    // update the address for the next run
    address += 0x1000; // = 4 kb
  }

  // Set the paging directory to point to kernel page table

  // First set the first 0-4 Mb
  m_PageDirectory[0].value              =
      reinterpret_cast<uint32_t>(&m_KernelPageTable[0]) - KVA;
  m_PageDirectory[0].bits4k.present     = 1;
  m_PageDirectory[0].bits4k.rw          = 1;
  m_PageDirectory[0].bits4k.accessed    = 1;
  // 4 kb page - not needed, but this clarifies syntax
  m_PageDirectory[0].bits4k.ps          = 0;

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
      printf(ANSI_FG_GREEN "[ok]\n" ANSI_NORMAL);*/
};

#define TRANSMAP_PERCPU_ENTRY(slot)   ((slot) + MY_CPU(id) * TRANSMAP_ENTRIES_PER_CPU)
#define TRANSMAP_ENTRY_VA(slot)       (TRANSMAP_WINDOW_KVA + (OOMTK_PAGE_SIZE * (slot)))
#define TRANSMAP_VA_ENTRY(va)         (((va) - TRANSMAP_WINDOW_KVA) / OOMTK_PAGE_SIZE)
#define TRANSMAP_ENTRY_SLOT(entry)    ((entry) % TRANSMAP_ENTRIES_PER_CPU)

void OPagingLegacy::transientMappingsInitialize()
{
  memset(m_TransientMappings, 0, OOMTK_PAGE_SIZE * TRANSMAP_PAGES/2);
  
  for (size_t i = 0; i < TRANSMAP_PAGES/2; i++)
  {
    const uint32_t offset = OOMTK_PAGE_SIZE * i;
    const kva_t va = TRANSMAP_WINDOW_KVA + i * 4*1024*1024;
    const uint32_t pa = ((reinterpret_cast<uint32_t>(&m_TransientMappings[0])) - KVA) + offset;
    
    uint32_t undx = PTE_PGDIR_NDX(va);
    m_PageDirectory[undx].value = pa;
    m_PageDirectory[undx].bits4k.present  = 1;
    m_PageDirectory[undx].bits4k.rw       = 1;
    m_PageDirectory[undx].bits4k.accessed = 1;
//     m_PageDirectory[undx].bits4k.dirty    = 1;
    m_PageDirectory[undx].bits4k.ps       = 0;
  }
}
