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
#include "ointerruptvector.h"

// Initialize the static members
OInterruptVector   OInterruptVector::m_vectorMap[NUM_VECTOR];
OInterruptVector * OInterruptVector::m_irqVector[NUM_IRQ];

OInterruptVector::OInterruptVector()
{
}

OInterruptVector::~OInterruptVector()
{
}

OInterruptVector::vector_hold_info_t OInterruptVector::grab()
{
  vector_hold_info_t hi = (vector_hold_info_t) m_stallQ.m_lock.grab();
  hi.oldFlags = IRQ::locallyDisableInterrupts();

  return hi;
}

void OInterruptVector::release (OInterruptVector::vector_hold_info_t hi)
{
  m_stallQ.m_lock.release ( (OSpinLock::hold_info_t) hi);
  IRQ::locallyEnableInterrupts (hi.oldFlags);
}

void OInterruptVector::setStatus (VectorStatus status)
{
  vector_hold_info_t hi = grab();

  if (m_type == vt_Interrupt)
  {
    if (status == vs_Live && m_status != vs_Live)
    {
      m_controller->unmask (this);
    }
    else
    {
      m_controller->mask (this);
      m_controller->ack (this);
    }
  }

  m_status = status;

  release (hi);
}


