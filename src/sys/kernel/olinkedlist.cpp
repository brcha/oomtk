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
#include "olinkedlist.h"

#include <cassert>

OLinkedList::OLinkedList()
  : m_next(this), m_prev(this)
{
}


OLinkedList::~OLinkedList()
{
}

bool OLinkedList::isEmpty()
{
  return (m_next == this);
}

void OLinkedList::unlink()
{
  m_next->m_prev = m_prev;
  m_prev->m_next = m_next;
  m_next = this;
  m_prev = this;
}

void OLinkedList::insertAfter(OLinkedList * newItem)
{
  assert (newItem.isEmpty);
  assert (m_next);
  assert (m_prev);
  
  newItem->m_next = m_next;
  newItem->m_prev = this;
  m_next->m_prev = newItem;
  m_next = newItem;
}

void OLinkedList::insertBefore(OLinkedList * newItem)
{
  assert (m_prev);
  m_prev->insertAfter(newItem);
}

OLinkedList * OLinkedList::next()
{
  return m_next;
}

OLinkedList * OLinkedList::prev()
{
  return m_prev;
}


