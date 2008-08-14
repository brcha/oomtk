#ifndef __OOMTKSYS_OPROCESS_H__
#define __OOMTKSYS_OPROCESS_H__
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
/** @file oprocess.h
 * @brief Process
 */

#include <OOMTK/OLinkedList>
#include <OOMTK/OAtomic>
#include <OOMTK/OAtomicPtr>
#include <OOMTK/OCPU>
#include <OOMTK/OMapping>

#include <types.h>
#include <cstddef>
#include <macros.h>
#include <config.h>

#include INC_ARCH(registers.h)

class OStallQueue;
class OReadyQueue;
class OVectorHandlers;

/**
 * @brief The process class
 *
 * @todo This class should be implemented, this is more like a forward declaration for now...
 */
class OProcess : public OLinkedList
{
  public:
    OProcess();
    ~OProcess();

    /**
     * @brief Reasons why the process cannot run
     */
    enum Issues
    {
      issue_Schedule      = 0x0001u,  ///< @brief Scheduling criteria unknown
      issue_Faulted       = 0x0002u,  ///< @brief Process has non-zero fault code
      issue_SingleStep    = 0x0004u,  ///< @brief Single step mode has been requested
      issue_Preempted     = 0x0008u,  ///< @brief Process has been preempted

      issue_NumericsUnit  = 0x1000u,  ///< @brief Process needs the FPU
      issue_VectorUnit    = 0x2000u,  ///< @brief Process needs the vector unit
      issue_SysCallDone   = 0x4000u,  ///< @brief System call has completed

      issue_OnLoad        = issue_Schedule, ///< @brief Issue set when process is loaded
    };

    /**
     * @brief Remove the process from the queue it is on and put it on the run queue
     *
     * If the process is not sleeping on the queue or it is on run queue already, just ignore the call.
     */
    void unsleep();
  protected:
    /** @brief Reasons why the process cannot run
     *
     * The issues field is a bitmask that identifies any condition that preclude this process
     * from making progress if dispatched. If the issues field is zero, the process is
     * immediately dispatchable as it has no issues. If the field is non-zero, some form of
     * additional handling is required before dispatching this process becomes possible.
     *
     * @see Issues
     */
    OAtomic     m_issues;

    cpuid_t     m_lastCPU;

    /** @brief Ready queue to go on after waking up
     *
     * A process can have different scheduling classes and therefore different ready queues.
     */
    OReadyQueue * m_readyQ;

    /// @brief The stall or ready queue the process is currently on.
    OStallQueue * m_onQ;

    /// @brief The CPU the process is currently running on
    OAtomicPtr<OCPU>  m_onCPU;

    /// @brief When to wake up (if sleeping)
//     Interval      m_wakeTime;

    /// @brief Top level mapping for this process
    OMapping  *   m_mapping;

    /// @brief Stall queue for all processes waiting to send to this one
//     OStallQueue   m_rcvWaitQ;

    /// @brief Non-zero when we aree in an extended IPC transaction with a peer
    OProcess    * m_ipcPeer;
    
    // Below are the process state variables:
    
    /// @brief Register set for current process
    regset_t    m_regs;

    friend class OReadyQueue;
    friend class OStallQueue;
    friend class OVectorHandlers;
    
    OFFSET_FRIENDLY;
};

#endif /* __OOMTKSYS_OPROCESS_H__ */
