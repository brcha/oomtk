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
#include "ophysicalmemorymanagement.h"
#include <ia32/pagesize.h>
#include <config.h>
#include <macros.h>
#include <types.h>

#include <fatal.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <OOMTK/OSpinLock>

#include "shellsort.h"

#define DEBUG_PMEM DEBUG_DISABLE

OPhysicalMemoryManagement::OPhysicalMemoryManagement() :
    m_nInfo(0)
{
  needPages.base    = (kpa_t)0u;
  needPages.bound   = ~((kpa_t)0u);
  needPages.align   = OOMTK_PAGE_SIZE;
  needPages.fromTop = true;
  needWords.base    = (kpa_t)0u;
  needWords.bound   = ~((kpa_t)0u);
  needWords.align   = sizeof(word_t);
  needWords.fromTop = true;
  needBytes.base    = (kpa_t)0u;
  needBytes.bound   = ~((kpa_t)0u);
  needBytes.align   = sizeof(uint8_t);
  needBytes.fromTop = true;
}


OPhysicalMemoryManagement::~OPhysicalMemoryManagement()
{
}

void OPhysicalMemoryManagement::initialize(kpa_t base, kpa_t bound)
{
  (void) newRegion (base, bound, cls_Addressable, use_Available, 0);
  
  needPages.base  = base;
  needPages.bound = bound;
  needWords.base  = base;
  needWords.bound = bound;
  needBytes.base  = base;
  needBytes.bound = bound;
}

OPhysicalMemoryManagement::info_t *
    OPhysicalMemoryManagement::overlapsRegion(kpa_t base, kpa_t bound, bool ignoreAvail)
{
  OSpinLock::hold_info_t hi = m_lock.grab();
  
  for (unsigned i = 0; i < PHYSMEM_NREGIONS; i++)
  {
    if (m_table[i].cls == cls_Unused)
      continue;
    
    if (ignoreAvail && m_table[i].use == use_Available)
      continue;
    
    // base falls within, bound anywhere
    if (m_table[i].base <= base && base < m_table[i].bound)
      return &m_table[i];
    
    // bound falls within, base anywhere
    if (m_table[i].base < bound && bound <= m_table[i].bound)
      return &m_table[i];
    
    // The above cases cover base and bound both within this region. Also check
    // if the region is fully contained by base and bound.
    if (base <= m_table[i].base && bound >= m_table[i].bound)
      return &m_table[i];
  }
  
  m_lock.release(hi);
  
  // If nothing is returned before, return NULL
  return NULL;
}

OPhysicalMemoryManagement::info_t *
    OPhysicalMemoryManagement::newRegion(kpa_t base, kpa_t bound, Class cls, Use use, const char * description)
{
  OSpinLock::hold_info_t hi = m_lock.grab();
  
  if (m_nInfo >= PHYSMEM_NREGIONS)
  {
    showAll();
    fatal("Physical memory regions list full!\n");
  }
  
  assert(m_nInfo < PHYSMEM_NREGIONS);
  assert(m_table[m_nInfo].cls == cls_Unused);
  
  info_t * pmi = &m_table[m_nInfo];
  m_nInfo ++;
  
  assert(pmi->cls == cls_Unused);
  assert(pmi->use == use_Available);
  assert(pmi->description == 0);
  assert(pmi->base == 0);
  assert(pmi->bound == 0);
  
  DEBUG_PMEM
      printf("Adding physical memory region [" OOMTK_KPA_FMT ", " OOMTK_KPA_FMT ") %s %s\n",
	     base, bound, describeClass(cls), describeUse(use));
  
  info_t * alt = overlapsRegion(base, bound, true);
  if (alt)
    fatal ("\t[" OOMTK_KPA_FMT ", " OOMTK_KPA_FMT ") overlaps with [" OOMTK_KPA_FMT ", " OOMTK_KPA_FMT ") %s %s!\n",
	   base, bound, alt->base, alt->bound, describeClass(cls), describeUse(use));
  
  // After all the check, do the actual allocation
  pmi->base = base;
  pmi->bound = bound;
  pmi->cls = cls;
  pmi->use = use;
  pmi->description = description; // TODO: Does this work? strcpy, anyone? but without malloc...?
  
  m_lock.release(hi);
  
  // And the final leap of faith, find me my region I just made:
  return findRegion(base);
}

const char * OPhysicalMemoryManagement::describeClass(Class cls)
{
  switch (cls)
  {
    case cls_Unused:
      return "unused";
    case cls_Addressable:
      return "addressable";
    case cls_RAM:
      return "RAM";
    case cls_NVRAM:
      return "NVRAM";
    case cls_ROM:
      return "ROM";
    default:
      assert(false);
      return 0;
  }
}

const char * OPhysicalMemoryManagement::describeUse(Use use)
{
  switch (use)
  {
    case use_Available:
      return "available";
    case use_BIOS:
      return "BIOS";
    case use_ACPI_NVS:
      return "ACPI NVS";
    case use_ACPI_DATA:
      return "ACPI DATA";
    case use_Kernel:
      return "Kernel";
    case use_ISLImage:
      return "ISLImage";
    case use_KHeap:
      return "KHeap";
    case use_KMap:
      return "KMap";
    case use_Pages:
      return "Pages";
    case use_PageTable:
      return "PageTable";
    case use_SMP:
      return "SMP";
    case use_Device:
      return "Device";
    case use_DevicePage:
      return "DevicePage";
    default:
      assert (false);
      return 0;
  }
}

kpa_t OPhysicalMemoryManagement::allocRegion(kpa_t base, kpa_t bound, Class cls, Use use, const char * description)
{
  if (base == bound)
    return 0;
  
  assert (base < bound);
  
  OSpinLock::hold_info_t hi = m_lock.grab();
  
  info_t * pmi = findRegion(base);
  
  if (!pmi)
  {
    m_lock.release(hi);
    return AllocFail;
  }
  
  if (pmi->base > base    ||
      pmi->bound < bound  ||
      pmi->base > bound   ||
      pmi->bound < bound)
  {
    // showAll();
    fatal("AllocRegion [" OOMTK_KPA_FMT "," OOMTK_KPA_FMT ") %s %s %s\n"
	  " PMI [" OOMTK_KPA_FMT ", " OOMTK_KPA_FMT ") %s %s\n",
          base, bound, describeClass(cls), describeUse(use), description,
	  pmi->base, pmi->bound, describeClass(pmi->cls), describeUse(pmi->use)
	 );
  }
  
  // Split the lower part, so that the region starts with base
  if (base != pmi->base)
  {
    assert (base > pmi->base);
    
    kpa_t obound = pmi->bound;
    pmi->bound = base;
    
    assert (base != obound);
    pmi = newRegion (base, obound, pmi->cls, pmi->use, pmi->description);
  }
  
  // Split the upper part, so that the region ends with bound
  assert (pmi->base == base);
  if (bound != pmi->bound)
  {
    assert (bound < pmi->bound);
    
    kpa_t obound = pmi->bound;
    pmi->bound = bound;
    
    assert (bound != obound);
    newRegion (bound, obound, pmi->cls, pmi->use, pmi->description);
  }
  
  DEBUG_PMEM
      printf("Alloc PhysicalMemoryRegion [" OOMTK_KPA_FMT ", " OOMTK_KPA_FMT ") %s %s\n",
	     base, bound, describeClass(cls), describeUse(use));
  
  assert (pmi->base == base);
  assert (pmi->bound == bound);
  pmi->use = use;
  pmi->cls = cls;
  pmi->description = description;
  
  cleanup();
  
  m_lock.release(hi);
  
  return base;
}

void OPhysicalMemoryManagement::showAll()
{
  OSpinLock::hold_info_t hi = m_lock.grab();
  
  for (unsigned i = 0; i < PHYSMEM_NREGIONS; i++)
  {
    if (m_table[i].cls == cls_Unused)
      continue;
    
    printf("[" OOMTK_KPA_FMT ", " OOMTK_KPA_FMT ") %-4s %-9s %s\n",
	   m_table[i].base, m_table[i].bound,
           describeClass(m_table[i].cls), describeUse(m_table[i].use),
	   m_table[i].description ? m_table[i].description : "");
  }
  
  m_lock.release(hi);
}

OPhysicalMemoryManagement::info_t *
    OPhysicalMemoryManagement::preferedRegion(OPhysicalMemoryManagement::info_t * reg1,
					      OPhysicalMemoryManagement::info_t * reg2)
{
  if (reg1 == 0)
    return reg2;
  
  if (reg2 && reg1->base < reg2->base)
    return reg2;
  
  return reg1;
}

OPhysicalMemoryManagement::info_t *
    OPhysicalMemoryManagement::chooseRegion(size_t bytes, const constraint_t & constraint)
{
  info_t * target = 0;
  
  for (unsigned i = 0; i < PHYSMEM_NREGIONS; i++)
  {
    info_t * pmi = &m_table[i];
    
    if (pmi->cls != cls_RAM)
      continue;
    if (pmi->use != use_Available)
      continue;
    
    kpa_t base = max(pmi->base, constraint.base);
    kpa_t bound = min(pmi->bound, constraint.bound);
    
    base = alignUp(base, constraint.align);
    
    if (base >= bound)
      continue;
    
    // The region (partially) overlaps the requested region. Try to suit the alligned space:
    if (constraint.fromTop)
    {
      kpa_t where = bound - bytes;
      where = alignDown(where, constraint.align);
      
      if (where < base)
	continue;
    } else
    {
      kpa_t where = alignUp(base, constraint.align);
      
      if (where + bytes > bound)
	continue;
    }
    
    target = preferedRegion(target, pmi);
  }
  
  return target;
}

kpa_t OPhysicalMemoryManagement::allocBytes(const constraint_t & constraint, size_t size, Use use, const char * description)
{
  OSpinLock::hold_info_t hi = m_lock.grab();
  
  info_t * pmi = chooseRegion(size, constraint);
  if (!pmi)
  {
    m_lock.release(hi);
    return AllocFail;
  }
  
  // Apply the constraint
  kpa_t bound = min(pmi->bound, constraint.bound);
  kpa_t base  = max(pmi->base,  constraint.base);
  
  kpa_t where;
  if (constraint.fromTop)
  {
    // The highest possible address for the bound
    where = alignDown(bound -size, constraint.align);
    assert(where >= pmi->base);
  } else
  {
    // The lowest possible address for the base
    where = alignUp(base, constraint.align);
    assert(where + size <= pmi->bound);
  }
  
  where = allocRegion(where, where + size, cls_RAM, use, description);
  
  m_lock.release(hi);
  
  return where;
}

kpsize_t OPhysicalMemoryManagement::available(const constraint_t & constraint, kpsize_t unitSize, bool contiguous)
{
  OSpinLock::hold_info_t hi = m_lock.grab();
  
  kpsize_t nUnits = 0;
  kpsize_t nContiguousUnits = 0;
  
  for (unsigned i = 0; i < PHYSMEM_NREGIONS; i++)
  {
    info_t * pmi = &m_table[i];
    
    if (pmi->cls != cls_RAM)
      continue;
    if (pmi->use != use_Available)
      continue;
    
    kpa_t base  = max(pmi->base, constraint.base);
    kpa_t bound = min(pmi->bound, constraint.bound);
    
    base = alignUp(base, constraint.align);
    
    if (base >= bound)
      continue;
    
    kpsize_t unitsHere = (bound -= base) / unitSize;
    
    if (nContiguousUnits < unitsHere)
      nContiguousUnits = unitsHere;
    
    nUnits += unitsHere;
  }
  
  m_lock.release(hi);
  
  return contiguous ? nContiguousUnits : nUnits;
}

bool OPhysicalMemoryManagement::sameDescription(const char * s1, const char * s2)
{
  if (s1 == s2)
    return true;
  if (s1 && s2)
    return (strcmp(s1, s2) == 0);
  return false;
}

void OPhysicalMemoryManagement::cleanup()
{
  shellsort(m_table, PHYSMEM_NREGIONS);
  
  // Check for mergeable regions:
  size_t ndx = 0;
  while (ndx < m_nInfo - 1)
  {
    info_t * pmi1 = &m_table[ndx];
    info_t * pmi2 = &m_table[ndx+1];
    
    assert (pmi1->cls != cls_Unused);
    assert (pmi2->cls != cls_Unused);
    
    // By construction, bases and bounds should always end up adjacent after sorting:
    assert (pmi1->bound == pmi2->base);
    
    if (pmi1->cls == pmi2->cls && // classes match
	pmi1->use == pmi2->use && // uses match
	mergeable(pmi1->use) && // use must be mergeable
	mergeable(pmi2->use) &&
	sameDescription(pmi1->description, pmi2->description)
       )
    {
      DEBUG_PMEM
	printf(
	    "Merging:\n"
	    "  [" OOMTK_KPA_FMT ", " OOMTK_KPA_FMT ") %s %s %s\n"
	    "  [" OOMTK_KPA_FMT ", " OOMTK_KPA_FMT ") %s %s %s\n",
            pmi1->base, pmi1->bound, describeClass(pmi1->cls), describeUse(pmi1->use),
            pmi1->description ? pmi1->description : "",
	    pmi2->base, pmi2->bound, describeClass(pmi2->cls), describeUse(pmi2->use),
	    pmi2->description ? pmi2->description : "");
      
      // These two regions should be merged:
      pmi1->bound = pmi2->bound;
      INIT_TO_ZERO(pmi2);
      
      shellsort(m_table, PHYSMEM_NREGIONS);
      
      m_nInfo --;
      
      DEBUG_PMEM
	  printf(
		 "Result:\n"
	         "  [" OOMTK_KPA_FMT ", " OOMTK_KPA_FMT ") %s %s %s (%d)\n",
	         pmi1->base, pmi1->bound, describeClass(pmi1->cls), describeUse(pmi1->use),
		 pmi1->description ? pmi1->description : "",
                 m_nInfo);
      
      continue;
    }
    
    ndx ++;
  }
}

bool operator >(OPhysicalMemoryManagement::info_t & obj1, OPhysicalMemoryManagement::info_t & obj2)
{
  assert((obj1.cls == OPhysicalMemoryManagement::cls_Unused) || (obj1.bound > obj1.base));
  assert((obj2.cls == OPhysicalMemoryManagement::cls_Unused) || (obj2.bound > obj2.base));
  
  if ((obj1.cls == OPhysicalMemoryManagement::cls_Unused && 
       obj2.cls != OPhysicalMemoryManagement::cls_Unused) ||
      (obj1.base > obj2.base))
    return true;
  return false;
}

bool operator <(OPhysicalMemoryManagement::info_t & obj1, OPhysicalMemoryManagement::info_t & obj2)
{
  assert((obj1.cls == OPhysicalMemoryManagement::cls_Unused) || (obj1.bound > obj1.base));
  assert((obj2.cls == OPhysicalMemoryManagement::cls_Unused) || (obj2.bound > obj2.base));
  
  if ((obj1.cls != OPhysicalMemoryManagement::cls_Unused &&
       obj2.cls == OPhysicalMemoryManagement::cls_Unused) ||
      (obj1.base < obj2.base))
    return true;
  return false;
}

bool operator ==(OPhysicalMemoryManagement::info_t & obj1, OPhysicalMemoryManagement::info_t & obj2)
{
  return (!(obj1 > obj2)) && (!(obj1 < obj2));
}

bool operator >=(OPhysicalMemoryManagement::info_t & obj1, OPhysicalMemoryManagement::info_t & obj2)
{
  return !(obj1 < obj2);
}

bool operator <=(OPhysicalMemoryManagement::info_t & obj1, OPhysicalMemoryManagement::info_t & obj2)
{
  return !(obj1 > obj2);
}

OPhysicalMemoryManagement * OPhysicalMemoryManagement::instance()
{
  static OPhysicalMemoryManagement _instance = OPhysicalMemoryManagement();
  
  return &_instance;
}


