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
#include "ocpu.h"
#include <string.h> // for INIT_TO_ZERO, that is memset()
#include <macros.h>
#include <config.h>
#include <types.h>

#include <OOMTK/OMutex>

// Define static members
OCPU OCPU::m_vector[MAX_NCPU];
size_t OCPU::m_ncpu = MAX_NCPU; // later updated

OCPU::OCPU()
{
}

void OCPU::initialize(cpuid_t ndx)
{
  OCPU * cpu = &OCPU::m_vector[ndx];
  
  INIT_TO_ZERO(cpu);
  cpu->m_id = ndx;
  cpu->m_transMetaMap = safeLeftShift(1ull, TRANSMAP_ENTRIES_PER_CPU) - 1ull;
//   cpu->curMap = &KernMapping;
  
  cpu->m_procMutexValue.set(OMutex::lockValue(0, OMutex::lty_Transaction, ndx));
//   cpu->wakeVectors = 0;
}
