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
#include "oagelist.h"

#include <cassert>

OAgeList::OAgeList()
  : m_count(0), m_needsFill(false), m_lowWater(0), m_highWater(0)
{
}


OAgeList::~OAgeList()
{
}

bool OAgeList::underLowWater()
{
  return m_count < m_lowWater;
}

bool OAgeList::underHighWater()
{
  return m_count < m_highWater;
}

bool OAgeList::isEmpty()
{
  return m_count == 0;
}

bool OAgeList::hasMoreThan(size_t count)
{
  return m_count > count;
}

OAgeable * OAgeList::oldest()
{
  if (m_list.isEmpty())
    return 0;
  return m_list.prev();
}

void OAgeList::remove(OAgeable * element)
{
  assert(m_count > 0);
  assert(element->m_list == this);
  
  element->unlink();
  element->m_list = NULL;
  m_count --;
}

bool OAgeList::isOldest(OAgeable * element)
{
  return m_list.prev() == element;
}

OAgeable * OAgeList::removeOldest()
{
  assert (!m_list.isEmpty());
  
  OAgeable * oldest = m_list.prev();
  remove(oldest);
  return oldest;
}

void OAgeList::addFront(OAgeable * newElement)
{
  m_list.insertAfter(newElement);
  newElement->m_list = this;
  m_count ++;
}

void OAgeList::addBack(OAgeable * newElement)
{
  m_list.insertBefore(newElement);
  newElement->m_list = this;
  m_count ++;
}

OAgeable::OAgeable()
  : OLinkedList(), m_list(NULL)
{
}

OAgeable::~ OAgeable()
{
}

