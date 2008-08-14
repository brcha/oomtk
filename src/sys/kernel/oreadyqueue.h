#ifndef __OOMTKSYS_OREADYQUEUE_H__
#define __OOMTKSYS_OREADYQUEUE_H__
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
/** @file oreadyqueue.h
 * @brief Ready queue
 */

#include <OOMTK/OStallQueue>
#include <OOMTK/OProcess>

/**
 * @brief Ready queue
 */
class OReadyQueue : public OStallQueue
{
  public:
    static OReadyQueue * mainRQ();
    
  protected:
    OReadyQueue();
    ~OReadyQueue();
    
  public:
    /**
     * @brief Add the @p process to this queue
     */
    void add(OProcess * process, bool atFront);
    /**
     * @brief Remove the @p process from the queue
     */
    void remove(OProcess * process);
};

#endif /* __OOMTKSYS_OREADYQUEUE_H__ */
