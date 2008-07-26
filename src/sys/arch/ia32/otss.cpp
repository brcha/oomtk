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
#include "otss.h"

#include <string.h>
#include <macros.h>

#include <ia32/selectors.h>

OTSS::OTSS()
{
}

OTSS * OTSS::instance()
{
  static OTSS _instance = OTSS();
  
  return &_instance;
}

void OTSS::initialize()
{
  INIT_TO_ZERO(&m_tss[0]);
  m_tss[0].ss0 = sel_KernelData;
  m_tss[0].ss1 = sel_NULL;
  m_tss[0].ss2 = sel_NULL;
  m_tss[0].ioMapBase = sizeof(TSS_t);
  
  for (size_t i = 1; i < MAX_NCPU; i++)
    memcpy(&m_tss[i], &m_tss[0], sizeof(m_tss[0]));
}

void OTSS::load()
{
  GNU_ASM("ltr    %%ax"
  : /* no output */
  : "a" (sel_TSS)
	 );
}


