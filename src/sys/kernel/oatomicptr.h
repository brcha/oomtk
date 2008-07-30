#ifndef __OOMTKSYS_OATOMICPTR_H__
#define __OOMTKSYS_OATOMICPTR_H__
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
/** @file oatomicptr.h
 * @brief Atomic manipulation of pointers
 */

#ifdef OOMTK_ARCH_IA32
# include <kernel/ia32/oatomicptr-ia32.h>
#endif /* OOMTK_ARCH_IA32 */

/**
 * @brief Atomic manipulation of pointers
 */
template <typename T>
class OAtomicPtr
{
  public:
    OAtomicPtr();
    ~OAtomicPtr();
    
    /**
     * @brief Get accessor
     * @returns the contained pointer
     */
    inline T * get()
    {
      return m_ptr;
    }
    
    /**
     * @brief Set accessor
     * @param value the new value
     */
    inline void set(T * value)
    {
      m_ptr = value;
    }
    
    /**
     * @brief Compare and swap pointers
     * @param oldValue old value (checked against current value)
     * @param newValue new value
     * @returns the value previously contained in the pointer
     */
    inline T * compareAndSwap(T * oldValue, T * newValue)
    {
#ifdef OOMTK_ARCH_IA32
      return (T*) OOMTK::IA32::compareAndSwapPtr((void**)&m_ptr, (void*)oldValue, (void*)newValue);
#endif
    }

  protected:
    T * m_ptr;
};

#endif /* __OOMTKSYS_OATOMICPTR_H__ */
