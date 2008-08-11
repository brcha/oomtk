#ifndef __OOMTKSYS_OSPINLOCK_H__
#define __OOMTKSYS_OSPINLOCK_H__
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
/** @file ospinlock.h
 * @brief Spinlock type
 */

#include <OOMTK/OMutex>

/**
 * @brief Spinlock type
 *
 * A spinlock is a mutex, but it must be acquired uncoditionally. In contrast to mutex,
 * spinlock acquisition will not yield and spinlocks must be explicitely released.
 */
class OSpinLock : virtual public OMutex
{
  public:
    OSpinLock();
    virtual ~OSpinLock();
    
    /**
     * @brief Grap a spinlock unconditionally
     * @returns hold_info_t structure for release of a spinlock
     * 
     * This will not yield to a higher priority process. This should only be used
     * in critical sections, when you know that no yield is possible Spinlocks may
     * not be recursively acquired.
     */
    virtual hold_info_t grab();
};

#endif /* __OOMTKSYS_OSPINLOCK_H__ */
