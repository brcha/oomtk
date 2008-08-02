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

#include "vpstring.h"

#include <string.h>

#include <macros.h>

#include INC_ARCH(pagesize.h)

#include <OOMTK/OPaging>

void memcpy_vtop(kpa_t dest_pa, void * source_va, size_t length)
{
  kva_t source = (kva_t) source_va;
  
  while (length > 0)
  {
    kpa_t offset = dest_pa & OOMTK_PAGE_ADDR_MASK;
    char * map = (char *)OPaging::instance()->map(dest_pa - offset);
    char * addr = map + offset;
    size_t count = min(length, OOMTK_PAGE_SIZE - offset);
    memcpy (addr, (void *)source, count);
    OPaging::instance()->unmap((kva_t)map);
    dest_pa += count;
    source += count;
    length -= count;
  }
}

void memcpy_ptov(void * dest_va, kpa_t source_pa, size_t length)
{
  kva_t dest = (kva_t) dest_va;
  while (length > 0)
  {
    kva_t offset = source_pa & OOMTK_PAGE_ADDR_MASK;
    char * map = (char *) OPaging::instance()->map(source_pa - offset);
    char * addr = map + offset;
    size_t count = min(length, OOMTK_PAGE_SIZE - offset);
    memcpy((void *) dest, addr, count);
    OPaging::instance()->unmap((kva_t) map);
    dest += count;
    source_pa += count;
    length -= count;
  }
}

void memcpy_ptop(kpa_t dest_pa, kpa_t source_pa, size_t length)
{
  while (length > 0)
  {
    kpa_t offset = dest_pa & OOMTK_PAGE_ADDR_MASK;
    char * map = (char *) OPaging::instance()->map(dest_pa - offset);
    char * addr = map + offset;
    size_t count = min(length, OOMTK_PAGE_SIZE - offset);
    memcpy_ptov(addr, source_pa, count);
    OPaging::instance()->unmap((kva_t) map);
    dest_pa += count;
    source_pa += count;
    length -= count;
  }
}

void memset_p(kpa_t pa, int value, size_t length)
{
  while (length > 0)
  {
    kpa_t offset = pa & OOMTK_PAGE_ADDR_MASK;
    char * map = (char *) OPaging::instance()->map(pa - offset);
    char * addr = map + offset;
    size_t count = min(length, OOMTK_PAGE_SIZE - offset);
    memset (addr, value, count);
    OPaging::instance()->unmap((kva_t) map);
    pa += count;
    length -= count;
  }
}
