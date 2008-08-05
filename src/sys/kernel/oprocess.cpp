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
#include "oprocess.h"

#include <cassert>

#if 0
#include <OOMTK/OSpinLock>
#include <OOMTK/OStallQueue>
#include <OOMTK/OReadyQueue>
#endif

OProcess::OProcess()
{
}

OProcess::~OProcess()
{
}

#if 0
void OProcess::unsleep()
{
  assert (m_onQ != 0);
  
  OStallQueue * sq;
  OReadyQueue * rq = OReadyQueue::mainRQ();
  OSpinLock::hold_info_t hi;
  for (;;)
  {
    sq = m_onQ;
    if (sq == 0 || sq == rq)
      return;
    hi = sq->m_lock.grab();
    if (sq == m_onQ)
      break;
    sq->m_lock.release(hi);
  }
  OSpinLock::hold_info_t rqhi = rq->m_lock.grab();
  unlink();
  m_onQ = rq;
  rq->m_head.insertBefore(this);
  rq->m_lock.release(rqhi);
  sq->m_lock.release(hi);
}
#endif