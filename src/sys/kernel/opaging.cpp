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
#include "opaging.h"

#include <OOMTK/OCPU>

#include <cstdio>

#include <ia32/pagesize.h>

#ifdef OOMTK_ARCH_IA32
# include "arch/ia32/opaging-ia32.h"
#endif

OPaging * OPaging::instance()
{
#ifdef  OOMTK_ARCH_IA32
  return OPagingIA32::instance();
#endif
}

void OPaging::transmapAdviseTlbFlush()
{
  DEBUG_PAGING printf("Transmap: flushed\n");
  
  OCPU::current()->m_transMetaMap |= OCPU::current()->m_transMetaMapReleased;
  OCPU::current()->m_transMetaMapReleased = 0;
}

uint32_t OPaging::transmapPerCPUentry(uint32_t slot)
{
  return slot + OCPU::current()->m_id * TRANSMAP_ENTRIES_PER_CPU;
}

kva_t OPaging::transmapEntryVA(uint32_t entry)
{
  return TRANSMAP_WINDOW_KVA + OOMTK_PAGE_SIZE * entry;
}

uint32_t OPaging::transmapVAentry(kva_t va)
{
  return (va - TRANSMAP_WINDOW_KVA) / OOMTK_PAGE_SIZE;
}

uint32_t OPaging::transmapEntrySlot(uint32_t entry)
{
  return entry % TRANSMAP_ENTRIES_PER_CPU;
}
