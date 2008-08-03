#ifndef __OOMTKSYS_OSTALLQUEUE_H__
#define __OOMTKSYS_OSTALLQUEUE_H__
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
/** @file ostallqueue.h
 * @brief Stall queue class
 */

#include <OOMTK/OSpinLock>
#include <OOMTK/OLinkedList>
#include <OOMTK/OProcess>

/**
 * @brief Stall queue class
 * 
 * Stall queues are standard link structures with a lock.
 */
class OStallQueue
{
  public:
    OStallQueue();
    ~OStallQueue();
    
    /**
     * @brief Check if queue is empty
     * @returns true if the queue is empty
     */
    bool isEmpty();
    
    /**
     * @brief Wake all processes on the stall queue
     */
    void wakeAll();
    
    /**
     * @brief Enqueue on the stall queue
     */
    void enqueue();
    
    /**
     * @brief Remove the @p process from the queue it is on and put it on the run queue
     * 
     * If the process is not sleeping on the queue or it is on run queue already, just ignore the call.
     */
    void unsleep(OProcess * process);
    
    void sleep();
    
  protected:
    OLinkedList   m_head;
    OSpinLock     m_lock;

};

#endif /* __OOMTKSYS_OSTALLQUEUE_H__ */
