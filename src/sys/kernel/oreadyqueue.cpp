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
#include "oreadyqueue.h"

#include <cassert>

OReadyQueue::OReadyQueue()
{
}


OReadyQueue::~OReadyQueue()
{
}

OReadyQueue * OReadyQueue::mainRQ()
{
  static OReadyQueue _mainRQ = OReadyQueue();
  return &_mainRQ;
}

void OReadyQueue::add(OProcess * process, bool atFront)
{
  OSpinLock::hold_info_t hi = m_lock.grab();
  process->m_onQ = this;
  if (atFront)
    m_head.insertAfter(process);
  else
    m_head.insertBefore(process);
  m_lock.release(hi);
}

void OReadyQueue::remove(OProcess * process)
{
  assert (process->m_onQ == this);
  
  OSpinLock::hold_info_t hi = m_lock.grab();
  assert (!process->isEmpty()); // check if the process link is empty
  process->m_onQ = NULL;
  process->unlink();
  m_lock.release(hi);
}



