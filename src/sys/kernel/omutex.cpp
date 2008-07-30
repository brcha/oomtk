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
#include "omutex.h"

#include <OOMTK/OCPU>

#include <cassert>
#include <fatal.h>

OMutex::OMutex()
  : m_atomic(0)
{
}


OMutex::~OMutex()
{
}

void OMutex::releaseAllProcessLocks()
{
  OCPU * cpu = OCPU::current();
  word_t currentMutexValue = cpu->m_procMutexValue.get();
  cpu->m_procMutexValue.set(OMutex::lockIncGeneration(currentMutexValue));
  cpu->m_shouldDefer.set(0);
}

bool OMutex::doTryLock(word_t currentValue, word_t * outValue)
{
  word_t against = 0;
  
  switch (OMutex::lockType(currentValue))
  {
    case OMutex::lty_Transaction:
    {
      size_t cpundx = OMutex::lockCPU(currentValue);
      assert (cpundx < OCPU::m_ncpu);
      
      OCPU * cpu = &OCPU::m_vector[cpundx];
      if (currentValue != cpu->m_procMutexValue.get())
      {
	// we can take the lock directly, it's been group-released
	against = currentValue;
	break;
      }
      
      if (cpu == OCPU::current())
      {
	// valid lock held by our CPU => allow the recursive lock
	*outValue = currentValue;
	return true;
      }
      break;
    }
    
    default:
      assert (currentValue == 0);
      break;
  }
  
  word_t cur = m_atomic.compareAndSwap(against, OCPU::current()->m_procMutexValue.get());
  
  *outValue = cur;
  return (cur == against);
}

bool OMutex::isHeld()
{
  word_t currentValue = m_atomic.get();
  return (currentValue == OCPU::current()->m_procMutexValue.get());
}

OMutex::hold_info_t OMutex::grab()
{
  word_t oldValue = m_atomic.get();
  
  while(1)
  {
    if (doTryLock(oldValue, &oldValue))
    {
      hold_info_t hi;
      hi.lockPtr = this;
      hi.oldValue = oldValue;
      return hi;
    }
    
    /*
     * A call to OMutex::grab() can be made under two conditions:
     * 
     *   1. There is a current process, and that process is running.
     *   2. There is NO current process, and we are trying to establish one.
     * 
     * Because of the second case, we must abandon rather than retry the
     * transaction if restart occurs. Because of the first case, we must check
     * to ensure that the current process goes onto the ready queue if such is
     * appropriate.
     */
    if (OCPU::current()->m_shouldDefer.get() == OCPU::current()->m_procMutexValue.get())
    {
      if (OCPU::current()->m_currentProcess)
      {
// 	assert (OCPU::current()->m_currentProcess->state.runState == PRS_RUNNING);
	
	/* Stick the abandoning process back on the ready queue, not at
	 * the front. Note that the invoking the ready queue logic here
	 * will invoke the mutex subsystem recursively, but the recursive invocation
	 * will be a spinlock acquisition, so that is ok.
	 */
// 	rq_add(&mainRQ, OCPU::current()->m_currentProcess, false);
      }
//       sched_abandon_transaction();
    }
    
    if (OMutex::lockType(oldValue) == OMutex::lty_Transaction)
    {
      size_t cpundx = OMutex::lockCPU(oldValue);
      assert (cpundx < OCPU::m_ncpu);
      OCPU * cpu = &OCPU::m_vector[cpundx];
      
      /* Contrary to appearances, it is NOT a bug that the target CPU priority might
       * move out from under us. In that case, the target CPU transaction ID will also
       * change, and we will re-attempt the acquisition next time around the loop.
       * This has the net result that any priority movement gets corrected by the
       * iteration.
       */
      if ((cpu->m_priority.get() < OCPU::current()->m_priority.get()) ||
	  ( (cpu->m_priority.get() == OCPU::current()->m_priority.get()) &&
	    (cpu->m_id > OCPU::current()->m_id) ))
      {
	cpu->m_shouldDefer.set(oldValue);
      }
    }
  }
}

bool OMutex::tryGrab(hold_info_t * hi)
{
  word_t oldValue = m_atomic.get();
  
  if (doTryLock(oldValue, &oldValue))
  {
    if (hi != NULL)
    {
      hi->lockPtr = this;
      hi->oldValue = oldValue;
    }
    return true;
  }
  return false;
}

void OMutex::release(hold_info_t hi)
{
  word_t currentValue = hi.lockPtr->m_atomic.get();
  
#ifndef NDEBUG
  if (currentValue != OCPU::current()->m_procMutexValue.get())
  {
    fatal("OMutex::release: cpu %d, " OOMTK_KVA_FMT ": is gen=%d:ty=%d:cpu=%d,\n"
	  "expected gen=%d:ty=%d:cpu=%d\n",
          OCPU::current()->m_id, hi.lockPtr,
	  OMutex::lockGeneration(currentValue),
	  OMutex::lockType(currentValue),
	  OMutex::lockCPU(currentValue),
	  OMutex::lockGeneration(OCPU::current()->m_procMutexValue.get()),
	  OMutex::lockType(OCPU::current()->m_procMutexValue.get()),
	  OMutex::lockCPU(OCPU::current()->m_procMutexValue.get()));
  }
#endif
  hi.lockPtr->m_atomic.set(hi.oldValue);
}


