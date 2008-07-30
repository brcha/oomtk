#ifndef __OOMTKSYS_OMUTEX_H__
#define __OOMTKSYS_OMUTEX_H__
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
/** @file omutex.h
 * @brief Mutual exclusion type
 */

#include <OOMTK/OAtomic>

#include <types.h>
#include <config.h>
#include <macros.h>


/**
 * @brief Mutual exclusion
 * 
 * A held lock ensures bot mutual exclusion and residency. Mutexes can
 * be acquired recursively. They can be selectively released one at the
 * time for hot cases, or they can be gang-released at the end of the
 * kernel operation.
 */
class OMutex
{
  public:
    OMutex();
    ~OMutex();
    
    struct hold_info_t
    {
      OMutex *  lockPtr;
      word_t    oldValue;
    };
    
    enum LockTypes {
      lty_Transaction   = 0x1u,
    };
    
    /**
     * @brief Encode @p generation, lock @p type and @p cpu into the lock value
     * @returns encoded lock value
     */
    static inline word_t lockValue(word_t generation, word_t type, cpuid_t cpu)
    {
      return (generation * 4 * MAX_NCPU) | ((type * MAX_NCPU) | cpu);
    }
    
    /**
     * @brief Decode lock type from lock @p value
     * @returns decoded lock type
     */
    static inline word_t lockType(word_t value)
    {
      return (value & (3 * MAX_NCPU)) / MAX_NCPU;
    }
    
    /**
     * @brief Decode CPU from lock @p value
     * @returns decoded CPU id
     */
    static inline cpuid_t lockCPU(word_t value)
    {
      return value & (MAX_NCPU - 1);
    }
    
    /**
     * @brief Decode lock generation from lock @p value
     * @returns the lock generation
     */
    static inline word_t lockGeneration(word_t value)
    {
      return value / (4 * MAX_NCPU);
    }
    
    /**
     * @brief Increment lock generation in lock @p value
     * @returns lock value with incremented generation
     */
    static inline word_t lockIncGeneration(word_t value)
    {
      return value + 4*MAX_NCPU;
    }
    
    /**
     * @brief Grab the mutex
     * @returns the hold_info_t structure, needed for mutex release
     */
    hold_info_t grab();
    
    /**
     * @brief Attempt to grab the mutex.
     * @param hi is filled with release info on success
     * @returns false on failure, true on success
     */
    bool tryGrab(hold_info_t * hi);
    
    /**
     * @brief Release a held mutex
     * @param hi a hold_info_t returned by most recent grab() or tryGrab()
     */
    void release(hold_info_t hi);
    
    /**
     * @brief Check if mutex is held by the inquiring CPU
     * @returns true if mutex is held by the inquiring CPU
     */
    bool isHeld();
    
    /**
     * @brief Release all process locks currently held by the process running on this CPU
     */
    void releaseAllProcessLocks();
    
  protected:
    /**
     * @brief Attempt to grab mutex once
     * @param currentValue value expected to be in the mutex
     * @param outValue value the lock should take upon release (if successful)
     * @returns true on success, false on failure
     */
    bool doTryLock(word_t currentValue, word_t * outValue);
  protected:
    OAtomic m_atomic;
};

#endif /* __OOMTKSYS_OMUTEX_H__ */
