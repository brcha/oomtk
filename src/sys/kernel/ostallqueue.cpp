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
#include "ostallqueue.h"

#include <OOMTK/OReadyQueue>
#include <OOMTK/OProcess>

OStallQueue::OStallQueue()
{
}


OStallQueue::~OStallQueue()
{
}

bool OStallQueue::isEmpty()
{
  OSpinLock::hold_info_t hi = m_lock.grab();
  bool result = m_head.isEmpty();
  m_lock.release(hi);
  
  return result;
}

OProcess * OStallQueue::removeFront()
{
  OProcess * p = NULL;
  
  OSpinLock::hold_info_t hi = m_lock.grab();
  if (!isEmpty())
  {
    p = m_head.next();
    p->unlink();
    p->m_onQ = NULL;
  }
  m_lock.release(hi);
  return p;
}

void OStallQueue::wakeAll()
{
  OReadyQueue * rq = OReadyQueue::mainRQ();
  
  OSpinLock::hold_info_t hi = m_lock.grab();
  if (!m_head.isEmpty())
  {
    OSpinLock::hold_info_t rqhi = rq->m_lock.grab();
    while (!m_head.isEmpty())
    {
      OProcess * p = m_head.next();
      p->m_onQ = rq;
      rq->m_head.insertBefore(p);
    }
    rq->m_lock.release(rqhi);
  }
  m_lock.release(hi);
}

void OStallQueue::enqueue()
{
  OProcess * p = OCPU::current()->m_currentProcess;
  
  OSpinLock::hold_info_t hi = m_lock.grab();
  p->m_onQ = this;
  m_head.insertAfter(p);
  m_lock.release(hi);
}

void OStallQueue::sleep()
{
  enqueue();
  // sched_abandon_transaction() TODO:
}



