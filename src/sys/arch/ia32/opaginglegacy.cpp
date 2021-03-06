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

#include <cstdio>
#include <ansi.h>
#include <cstring>
#include <cassert>
#include <ffs.h>

#include <config.h>

#include <ia32/pagesize.h>
#include <ia32/CR.h>

#include <OOMTK/OCPU>
#include <OOMTK/OPhysicalMemoryManagement>

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
    OPagingIA32()
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

void OPagingLegacy::transientMappingsInitialize()
{
  memset(m_TransientMappings, 0, OOMTK_PAGE_SIZE * TRANSMAP_PAGES/2);
  
  for (size_t i = 0; i < TRANSMAP_PAGES/2; i++)
  {
    const uint32_t offset = OOMTK_PAGE_SIZE * i;
    const kva_t va = TRANSMAP_WINDOW_KVA + i * 4*1024*1024;
    const uint32_t pa = ((reinterpret_cast<uint32_t>(&m_TransientMappings[0])) - KVA) + offset;
    
    uint32_t undx = pdeIndex(va);
    m_PageDirectory[undx].value = pa;
    m_PageDirectory[undx].bits4k.present  = 1;
    m_PageDirectory[undx].bits4k.rw       = 1;
    m_PageDirectory[undx].bits4k.accessed = 1;
//     m_PageDirectory[undx].bits4k.dirty    = 1;
    m_PageDirectory[undx].bits4k.ps       = 0;
  }
}


kva_t OPagingLegacy::map(kpa_t pa)
{
  assert ((pa % OOMTK_PAGE_SIZE) == 0);
  assert (pa < PTE_PADDR_BOUND);
  
  uint32_t ndx = ffsll(OCPU::current()->m_transMetaMap);
  if (ndx == 0)
  {
    // Grab back just one at the time and use selective flush instruction.
    ndx = ffsll(OCPU::current()->m_transMetaMapReleased);
    assert (ndx);
    uint32_t slot = ndx - 1;
    uint32_t entry = transmapPerCPUentry(slot);
    OCPU::current()->m_transMetaMapReleased &= ~(1u << slot);
    // local_tlb_flushva(KernMapping.asid, transmapEntryVA(entry));
  }
  
  assert (ndx);
  
  uint32_t slot = ndx - 1;
  uint32_t entry = transmapPerCPUentry(slot);
  
  m_TransientMappings[entry].value = (uint32_t)pa;
  m_TransientMappings[entry].bits4k.present = 1;
  m_TransientMappings[entry].bits4k.rw = 1;
  m_TransientMappings[entry].bits4k.accessed = 1;
  m_TransientMappings[entry].bits4k.dirty = 1;
  
  kva_t va = transmapEntryVA(entry);
  OCPU::current()->m_transMetaMap &= ~(1u << slot);
  
  DEBUG_PAGING printf("Transmap: map va=" OOMTK_KVA_FMT " to pa=" OOMTK_KPA_FMT "\n", va, pa);
  
  return va;
}

void OPagingLegacy::unmap(kva_t va)
{
  uint32_t entry = transmapVAentry(va);
  uint32_t slot = transmapEntrySlot(entry);
  
  assert ((OCPU::current()->m_transMetaMap & (1u << slot)) == 0);
  assert ((OCPU::current()->m_transMetaMapReleased & (1u << slot)) == 0);
  
  m_TransientMappings[entry].value = 0;
  
  OCPU::current()->m_transMetaMapReleased |= (1u << slot);
  
  DEBUG_PAGING printf("Transmap: unmap va=" OOMTK_KVA_FMT "\n", va);
}

#define LARGE_PAGE_SIZE (OOMTK_PAGE_SIZE * NPTE_PER_PAGE)

void OPagingLegacy::kmap(kva_t va, kpa_t pa, size_t size, uint32_t perms, bool remap)
{
  assert (va >= KVA);
  assert ((size % OOMTK_PAGE_SIZE) == 0);
  
  while (size)
  {
    uint32_t undx = pdeIndex(va);
    
    if (!HavePSE)
      goto smallPageMapping;
    
    if (((va | pa) % LARGE_PAGE_SIZE) || (size < LARGE_PAGE_SIZE))
      goto smallPageMapping;
    
    assert (m_PageDirectory[undx].bits4m.userSuper == 0);
    
    // If I can't remap, and there already is a page without page-size activated, make a small page :(
    if (m_PageDirectory[undx].bits4m.present && !m_PageDirectory[undx].bits4m.ps && !remap)
      goto smallPageMapping;
    
    if (m_PageDirectory[undx].bits4m.present && !remap)
      fatal ("Attempt to remap existing mapping at " OOMTK_KVA_FMT "!\n", va);
    
    m_PageDirectory[undx].value = pa; // clear everything else
    m_PageDirectory[undx].bits4m.present  = (perms ? 1 : 0);
    m_PageDirectory[undx].bits4m.rw       = (perms & KMAP_W) ? 1 : 0;
    m_PageDirectory[undx].bits4m.pcd      = (perms & KMAP_NC) ? 1 : 0;
    m_PageDirectory[undx].bits4m.accessed = 1;
    m_PageDirectory[undx].bits4m.dirty    = 1;
    m_PageDirectory[undx].bits4m.ps       = 1;
    
    va += LARGE_PAGE_SIZE;
    pa += LARGE_PAGE_SIZE;
    size -= LARGE_PAGE_SIZE;
    
    continue;
    
smallPageMapping:
    if (((va | pa) % OOMTK_PAGE_SIZE) || (size < OOMTK_PAGE_SIZE))
    fatal ("Bad alignment or size in kmap.\n");
    
    uint32_t lndx = pteIndex(va);
    
    if (!m_PageDirectory[undx].bits4k.present || m_PageDirectory[undx].bits4k.ps)
    {
      // TODO: This needs to call heap_alloc_page_frame()
      OPhysicalMemoryManagement * pmm = OPhysicalMemoryManagement::instance();
      kpa_t pa = pmm->allocBytes(pmm->needPages, OOMTK_PAGE_SIZE, pmm->use_KMap, "Kernel page table");
      PTE_t * pgtbl = (PTE_t *)map(pa);
      
      if (m_PageDirectory[undx].bits4k.ps)
      {
	uint32_t pt_base = m_PageDirectory[undx].bits4k.pt_base;
	for (size_t i = 0; i < NPTE_PER_PAGE; i++)
	{
	  pgtbl[i].value = m_PageDirectory[undx].value; // copy all bits
	  pgtbl[i].bits4k.page_base = pt_base + i;
	}
      } else
      {
	memset(pgtbl, 0, OOMTK_PAGE_SIZE);
      }
      
      unmap((kva_t) pgtbl);
      
      // Setup a generic upper-level mapping entry for all kernel uses. Permissions will be
      // determined at the lower level.
      m_PageDirectory[undx].value = pa;
      m_PageDirectory[undx].bits4k.present  = 1;
      m_PageDirectory[undx].bits4k.rw       = 1;
      m_PageDirectory[undx].bits4k.accessed = 1;
      m_PageDirectory[undx].bits4k.ps       = 0;
    }
    
    assert (m_PageDirectory[undx].bits4k.userSuper == 0 &&
	m_PageDirectory[undx].bits4k.present == 1 &&
	m_PageDirectory[undx].bits4k.rw      == 1 &&
	m_PageDirectory[undx].bits4k.ps      == 0);
    
    kpa_t lowerTableAddr = frameToKpa(m_PageDirectory[undx].bits4k.pt_base);
    
    PTE_t * pgtbl = (PTE_t *) map(lowerTableAddr);
    
    pgtbl[lndx].bits4k.page_base  = kpaToFrame(pa);
    pgtbl[lndx].bits4k.present    = (perms ? 1 : 0);
    pgtbl[lndx].bits4k.rw         = (perms & KMAP_W) ? 1 : 0;
    pgtbl[lndx].bits4k.pcd        = (perms & KMAP_NC) ? 1 : 0;
    pgtbl[lndx].bits4k.accessed   = 1;
    pgtbl[lndx].bits4k.dirty      = 1;
    
    unmap((kva_t) pgtbl);
    
    va += OOMTK_PAGE_SIZE;
    pa += OOMTK_PAGE_SIZE;
    size -= OOMTK_PAGE_SIZE;
  }
}