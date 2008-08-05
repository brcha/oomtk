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

#include <OOMTK/OAtomic>
#include <OOMTK/OProcess>

class OMutex;
class OPaging;

#ifdef OOMTK_ARCH_IA32
class OPagingIA32;
class OPagingLegacy;
class OPagingPAE;
class OACPI;
#endif

// class OReadyQueue;
// class OStallQueue;

/**
 * @brief Every CPU has it's own cpu class.
 */
class OCPU
{
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
    
    /**
     * @brief Probe the CPUs present on this machine
     * @returns the number of available CPUs
     * 
     * @note This method has architecture dependant implementation.
     */
    static size_t probeCPUs();
    
  protected:
    /**
     * @brief Mutex for locks held by current process on this CPU.
     * 
     * @note A change of this member has the side effect of group-releasing all
     * spinlocks and mutexes currently held by this CPU. See OMutex::tryLock()
     * for more info.
     */
    OAtomic       m_procMutexValue;
    
    /**
     * @brief Process that is currently running on this CPU
     */
    OProcess *    m_currentProcess;
    
    /**
     * @brief Unique identifier for this CPU
     */
    cpuid_t       m_id;
    
    /**
     * @brief Bitmap of this CPU's available transmap entries
     */
    transmeta_t   m_transMetaMap;
    
    /**
     * @brief Bitmap of this CPU's available transmap entries that have
     * been released, but not yet flushed from the TLB
     */
    transmeta_t   m_transMetaMapReleased;
    
    /**
     * @brief Per-CPU kernel stack reload address
     */
    kva_t         m_topOfStack;
    
    /**
     * @brief If shouldDefer matches procMutexValue, this CPU has been asked
     * to get out of the way if it cannot aquire a mutex immediately.
     */
    OAtomic       m_shouldDefer;
    
    /**
     * @brief True iff this CPU is present
     */
    bool          m_present;
    
    /**
     * @brief True iff this CPU has been started
     * 
     * @note When CPU is started, it cannot be stopped.
     */
    bool          m_active;
    
    /**
     * @brief Priority of current process on CPU
     */
    OAtomic       m_priority;
    
    /**
     * @brief Action flags for this CPU
     */
    OAtomic       m_flags;
    
    /**
     * @brief Mapping context currently loaded on this CPU
     */
    // Mapping * curMap;
    
    /**
     * @brief Linked list of interrupt vector entries that need to be awaken.
     */
    // Something * m_wakeVectors;
    
    /**
     * @brief The number of CPUs
     * 
     * It is <= MAX_NCPU
     */
    static size_t m_ncpu;
    
    /**
     * @brief Vector of all CPU structures
     */
    static OCPU m_vector[MAX_NCPU] asm("cpu_vector");
    
    // Friendly classes
    friend class OMutex;
    friend class OPaging;
#ifdef OOMTK_ARCH_IA32
    friend class OPagingIA32;
    friend class OPagingLegacy;
    friend class OPagingPAE;
    friend class OACPI;
#endif
//     friend class OReadyQueue;
//     friend class OStallQueue;
};

#endif /* __OOMTKSYS_OCPU_H__ */
