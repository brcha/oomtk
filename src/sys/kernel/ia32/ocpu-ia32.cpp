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

#include <OOMTK/OCPU>

#include <ia32/pagesize.h>
#include <ia32/archcpu.h>
#include <config.h>

#include <cstdio>
#include <cstddef>
#include <cassert>

#include <arch/ia32/oacpi.h>

#include <OOMTK/OPhysicalMemoryManagement>
#include <OOMTK/OPaging>

OCPU * OCPU::current()
{
#if MAX_NCPU > 1
  register uint32_t sp asm("esp");
  
  sp &= ~((KSTACK_NPAGES * OOMTK_PAGE_SIZE) - 1);
  return *((OCPU **) sp);
#else
  // Only one CPU, so no "detecting" which CPU active is needed.
  return &OCPU::m_vector[0];
#endif
}

arch_cpu_t archcpu_vector[MAX_NCPU];

extern "C" uint32_t cpu0_kstack_hi[];

size_t OCPU::probeCPUs()
{
  size_t ncpu = OACPI::instance()->probeCpus();
  if (ncpu)
    printf ("ACPI reports %d CPUs\n", ncpu);
  
  if (ncpu == 0)
    ncpu = 1; // there must be at least one CPU :)

  // BSP is present and started:
  m_vector[0].m_present     = true;
  m_vector[0].m_active      = true;
  m_vector[0].m_topOfStack  = (kva_t) cpu0_kstack_hi;
  
  m_ncpu = ncpu;
  
  assert (m_ncpu <= MAX_NCPU);
  
  size_t nPage = KSTACK_NPAGES * (m_ncpu - 1);
  
  OPhysicalMemoryManagement * pmm = OPhysicalMemoryManagement::instance();
  kpa_t pa = pmm->allocBytes(pmm->needPages, OOMTK_PAGE_SIZE * nPage, pmm->use_Kernel, "SMP CPU Stack");
  OPaging * paging = OPaging::instance();
  paging->kmap(SMP_STACK_VA, pa, nPage * OOMTK_PAGE_SIZE, OPaging::KMAP_W, false);
  
  for (size_t i = 1; i < ncpu; i++)
  {
    kva_t stack_va  = SMP_STACK_VA + (i-1)*(KSTACK_NPAGES * OOMTK_PAGE_SIZE);
    kva_t stack_top = stack_va + KSTACK_NPAGES * OOMTK_PAGE_SIZE;
    m_vector[i].m_topOfStack = stack_top;
    
    *((OCPU **) stack_va) = &m_vector[i];
  }
  
  return m_ncpu;
}
