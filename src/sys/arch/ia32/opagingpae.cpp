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

#include "opagingpae.h"

#include <cstdio>
#include <ansi.h>
#include <fatal.h>
#include <cstring>
#include <ffs.h>
#include <cassert>

// #include <CPUID.h>

#include <ia32/pagesize.h>
#include <ia32/CR.h>

#include <OOMTK/OCPU>
#include <OOMTK/OPhysicalMemoryManagement>

/**
 * @brief Get the one instance of paging class
 * \returns the instance
 */
OPagingPAE * OPagingPAE::instance()
{
  static OPagingPAE paging = OPagingPAE();

  return &paging;
};

/**
 * \defgroup Mappings
 * @brief Mappings as declared in linker.ia32.ld.S + Kernel's PDPT
 * @{
 */

/**
 * @brief Kernel's PDPT
 *
 * Must be declared in the .data.percpu because of SMP. Like this it is aligned to
 * CPU cache size, so the CPUs could easily cache only what they need.
 *
 * This approach comes from Coyotos, and I hope it will work nicely (Coyotos doesn't
 * support SMP right now and neither does OOMTK, but I hope to support it as soon as
 * possible, and I found out that QEmu can emulate more than one CPU, so I will be
 * able to test it).
 */
uint64_t KernPDPT[4];

extern kva_t KernPageDir[0];    ///< @brief Master paging directory
extern kva_t KernPageTable[0];  ///< @brief Kernel paging table
extern kva_t TransientMap[0];   ///< @brief Transient mappings page

/** @} */ // Mappings

/**
 * @brief Protected constructor
 */
OPagingPAE::OPagingPAE() :
    OPagingIA32()
{
  m_KernelPDPT        = reinterpret_cast<PDPTE_t*>(&KernPDPT[0]);
  m_PageDirectory     = reinterpret_cast<PDE_t*>  (&KernPageDir[0]);
  m_KernelPageTable   = reinterpret_cast<PTE_t*>  (&KernPageTable[0]);
  m_TransientMappings = reinterpret_cast<PTE_t*>  (&TransientMap[0]);
};

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
void OPagingPAE::setup()
{
/*  DEBUG_PAGING
      printf("Starting IA32 PAE paging ... ");

  // Map first [0, 2M]
  uint64_t address = 0u;
  for (size_t i = 0; i < 512; i++)
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

  // Set the paging directory to point to kernel's page table

  // First set the first 0 - 2 Mb
  PageDirectory[0].value            =
      reinterpret_cast<uint32_t>(&KernelPageTable[0]) - KVA;
  PageDirectory[0].bits4k.present   = 1;
  PageDirectory[0].bits4k.rw        = 1;
  PageDirectory[0].bits4k.accessed  = 1;
  // 4 kb page - not needed, but clarifies the code
  PageDirectory[0].bits4k.ps        = 0;

  // Put the transient mappings to the top 12 Mb
  // Here that is entry no 0xFD0
  PageDirectory[0xFD0].value          =
      reinterpret_cast<uint32_t>(&TransientMappings[0]) - KVA;
  PageDirectory[0xFD0].bits4k.present = 1;
  PageDirectory[0xFD0].bits4k.rw      = 1;

  // Now set up the PDPT <- this relies on KVA = 0xC0000000, so I will not change that
  KernelPDPT[0].value         =
      reinterpret_cast<uint32_t>(&PageDirectory[0]) - KVA;
  KernelPDPT[0].bits.present  = 1;
  KernelPDPT[1].value         = 0u;
  KernelPDPT[2].value         = 0u;
  KernelPDPT[3].value         =
      reinterpret_cast<uint32_t>(&PageDirectory[0]) - KVA;
  KernelPDPT[3].bits.present  = 1;

  // Load up KernelPDPT into CR3
  PDPT_Register_t CR3;
  CR3.value = reinterpret_cast<uint32_t>(&KernelPDPT[0]) - KVA;
  //CR3.bits.no need to setup anything
  cr3(CR3.value);

  // Setup the CR4
  uint32_t CR4 = cr4();

  CPUID * cpuid = CPUID::instance();

  cpuid->identify(); // identify the CPU if not already identified
  if (!cpuid->has(CPUID::PAE)) // why was this class called if CPU doesn't have PAE?
    // Halt the CPU
    fatal(ANSI_FG_RED "[failed]\n" ANSI_NORMAL);
  CR4 |= CR4_PAE;
  if (cpuid->has(CPUID::PSE)) // Page Size Extension
      CR4 |= CR4_PSE;
  if (cpuid->has(CPUID::PGE)) // Page Global Extension
      CR4 |= CR4_PGE;

  // store the new value of CR4
  cr4(CR4);

  // Now enable paging
  uint32_t CR0 = cr0();

  CR0 |= CR0_PG;  // paging
  CR0 |= CR0_AM;  // alignment mask
  CR0 |= CR0_WP;  // write protect
  CR0 |= CR0_ET;  // extension type
  CR0 |= CR0_TS;  // task switched
  CR0 |= CR0_MP;  // monitor coprocessor
  CR0 |= CR0_PE;  // protection enable (should have been enabled by grub)

  cr0(CR0);

  DEBUG_PAGING
      printf(ANSI_FG_GREEN "[ok]\n" ANSI_NORMAL);*/
};

void OPagingPAE::transientMappingsInitialize()
{
  memset(m_TransientMappings, 0, OOMTK_PAGE_SIZE * TRANSMAP_PAGES);
  
  for (size_t i = 0; i < TRANSMAP_PAGES; i++)
  {
    const uint32_t offset = OOMTK_PAGE_SIZE * i;
    const kva_t va = TRANSMAP_WINDOW_KVA + i * 2*1024*1024;
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


kva_t OPagingPAE::map(kpa_t pa)
{
  assert ((pa % OOMTK_PAGE_SIZE) == 0);
  assert (pa < PAE_PADDR_BOUND);
  
  uint32_t ndx = ffsll(OCPU::current()->m_transMetaMap);
  if (ndx == 0)
  {
    // Grab back just one at the time and use selective flush instruction.
    ndx = ffsll(OCPU::current()->m_transMetaMapReleased);
    assert (ndx);
    uint32_t slot = ndx - 1;
    uint32_t entry = transmapPerCPUentry(slot);
    OCPU::current()->m_transMetaMapReleased &= ~(1u << slot);
    // local_tlb_flushva(KernMapping.asid, transmapEntryVA(entry)); (TODO)
  }
  
  assert (ndx);
  
  uint32_t slot = ndx - 1;
  uint32_t entry = transmapPerCPUentry(slot);
  
  m_TransientMappings[entry].value = pa;
  m_TransientMappings[entry].bits4k.present = 1;
  m_TransientMappings[entry].bits4k.rw = 1;
  m_TransientMappings[entry].bits4k.accessed = 1;
  m_TransientMappings[entry].bits4k.dirty = 1;
  
  kva_t va = transmapEntryVA(entry);
  OCPU::current()->m_transMetaMap &= ~(1u << slot);
  
  DEBUG_PAGING printf("Transmap: map va=" OOMTK_KVA_FMT " to pa=" OOMTK_KPA_FMT "\n", va, pa);
  
  return va;
}

void OPagingPAE::unmap(kva_t va)
{
  uint32_t entry = transmapVAentry(va);
  uint32_t slot = transmapEntrySlot(entry);
  
  assert ((OCPU::current()->m_transMetaMap & (1u << slot)) == 0);
  assert ((OCPU::current()->m_transMetaMapReleased & (1u << slot)) == 0);
  
  m_TransientMappings[entry].value = 0;
  
  OCPU::current()->m_transMetaMapReleased |= (1u << slot);
  
  DEBUG_PAGING printf("Transmap: unmap va=" OOMTK_KVA_FMT "\n", va);
}

#define LARGE_PAGE_SIZE (OOMTK_PAGE_SIZE * NPAE_PER_PAGE)

void OPagingPAE::kmap(kva_t va, kpa_t pa, size_t size, uint32_t perms, bool remap)
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
    
    assert (pdpteIndex(va) == 3); // here is the kernel, kmap doesn't map outside kernel va
    
    assert (m_PageDirectory[undx].bits2m.userSuper == 0);
    
    if (m_PageDirectory[undx].bits2m.present && !m_PageDirectory[undx].bits2m.ps && !remap)
      fatal ("Attempt to remap existing mapping at " OOMTK_KVA_FMT "!\n", va);
    
    m_PageDirectory[undx].value = pa; // clear all permissions in the process
    m_PageDirectory[undx].bits2m.present  = (perms ? 1 : 0);
    m_PageDirectory[undx].bits2m.rw       = (perms & KMAP_W) ? 1 : 0;
    m_PageDirectory[undx].bits2m.pcd      = (perms & KMAP_NC) ? 1 : 0;
    m_PageDirectory[undx].bits2m.accessed = 1;
    m_PageDirectory[undx].bits2m.dirty    = 1;
    m_PageDirectory[undx].bits2m.ps       = 1;
    
    va += LARGE_PAGE_SIZE;
    pa += LARGE_PAGE_SIZE;
    size -= LARGE_PAGE_SIZE;
    
    continue;
    
smallPageMapping:
    if (((va | pa) % OOMTK_PAGE_SIZE) || (size < OOMTK_PAGE_SIZE))
      fatal ("Bad alignment or size in kmap\n");
    
    assert (pdpteIndex(va) == 3); // here is the kernel
    uint32_t lndx = pteIndex(va);
    
    if (!m_PageDirectory[undx].bits4k.present || m_PageDirectory[undx].bits2m.ps)
    {
      // TODO: this needs to call heap_alloc_page_frame instead after starting the system
      OPhysicalMemoryManagement * pmm = OPhysicalMemoryManagement::instance();
      kpa_t pa = pmm->allocBytes(pmm->needPages, OOMTK_PAGE_SIZE, pmm->use_KMap, "Kernel page table");
      
      PTE_t * pgtbl = (PTE_t *) map(pa);
      
      if (m_PageDirectory[undx].bits4k.ps)
      {
	uint32_t pt_base = m_PageDirectory[undx].bits4k.pt_base;
	for (size_t i = 0; i < NPAE_PER_PAGE; i++)
	{
	  pgtbl[i].value = m_PageDirectory[undx].value;
	  pgtbl[i].bits4k.page_base = pt_base + i;
	}
      } else
      {
	memset (pgtbl, 0, OOMTK_PAGE_SIZE);
      }
      
      unmap((kva_t) pgtbl);
      
      // Setup generic upper level mapping and leave permissions to lower level
      m_PageDirectory[undx].value = pa; // clear all permissions
      m_PageDirectory[undx].bits4k.present  = 1;
      m_PageDirectory[undx].bits4k.rw       = 1;
      m_PageDirectory[undx].bits4k.accessed = 1;
      m_PageDirectory[undx].bits4k.ps       = 0;
    }
      
    assert (m_PageDirectory[undx].bits4k.userSuper == 0 &&
	m_PageDirectory[undx].bits4k.present == 1 &&
	m_PageDirectory[undx].bits4k.rw      == 1 &&
	m_PageDirectory[undx].bits4k.ps      == 0);
    
    kpa_t lowerTableAddr = frametoKpa(m_PageDirectory[undx].bits4k.pt_base);
    PTE_t * pgtbl = (PTE_t *) map(lowerTableAddr);
    
    pgtbl[lndx].value = 0;
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