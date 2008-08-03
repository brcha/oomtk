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
#include "ofreelist.h"

OFreeList::OFreeList()
{
  m_next.set(NULL);
}


OFreeList::~OFreeList()
{
}

OFreeListItem * OFreeList::alloc()
{
  OFreeListItem * next = m_next.get();
  
  for (;;)
  {
    OFreeListItem * was = m_next.compareAndSwap(next, next->m_next);
    if (was == next)
    {
      if (next)
	next->m_next = 0;
      return next;
    }
    next = was;
  }
}

void OFreeList::insert(OFreeListItem * element)
{
  OFreeListItem * first = m_next.get();
  
  for (;;)
  {
    element->m_next = first;
    
    OFreeListItem * was = m_next.compareAndSwap(first, element);
    if (was == first)
      return;
    
    first = was;
  }
}


