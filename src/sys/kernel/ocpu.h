#ifndef __OOMTKSYS_OCPU_H__
#define __OOMTKSYS_OCPU_H__
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
/** @file ocpu.h
 * @brief Per-CPU classes and structures.
 */

#include <types.h>
#include <config.h>
#include <cstddef>

#include <OOMTK/OProcess>

/**
 * @brief Every CPU has it's own cpu class.
 */
class OCPU{
  protected:
    /**
     * @brief Protected constructor (not for use by mortals)
     */
    OCPU();

  public:
    /**
     * @brief Initialize the cpu_vector structure
     */
    static void initialize(cpuid_t ndx);
    
    /**
     * @brief Get the OCPU * for the current CPU
     * @note This method has architecture dependant implementation.
     */
    static OCPU * current();
    
  private:
    /**
     * @brief Mutex for locks held by current process on this CPU.
     */
    // Atomic m_procMutexValue
    
    /**
     * @brief Process that is currently running on this CPU
     */
    OProcess * m_current;
    
    /**
     * @brief Unique identifier for this CPU
     */
    cpuid_t   m_id;
    
    /**
     * @brief Per-CPU kernel stack reload address
     */
    kva_t     m_topOfStack;
    
    /**
     * @brief True iff this CPU is present
     */
    bool      m_present;
    
    /**
     * @brief True iff this CPU has been started
     * 
     * @note When CPU is started, it cannot be stopped.
     */
    bool      m_active;
    
    /**
     * @brief Linked list of vector entries that need to be awaken.
     */
    // Something * m_wakeVectors;
    
    /**
     * @brief The number of CPUs
     * 
     * It is <= MAX_NCPU
     */
    static size_t ncpu;
};

extern "C" OCPU cpu_vector[MAX_NCPU];

#endif /* __OOMTKSYS_OCPU_H__ */
