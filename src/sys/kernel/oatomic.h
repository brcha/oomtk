#ifndef __OOMTKSYS_OATOMIC_H__
#define __OOMTKSYS_OATOMIC_H__
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
/** @file oatomic.h
 * @brief Atomic operations
 */

#include <macros.h>
#include <config.h>
#include <types.h>

/**
 * @brief Manipulation of atomic words
 */
class OAtomic
{
  public:
    OAtomic(word_t init = 0);
    ~OAtomic();
    
    /**
     * @brief Atomic compare and swap
     * @param oldValue old value
     * @param newValue new value
     * @returns word prior to operation
     * 
     * This is one of the basic atomic operations. If current word value is @p oldValue,
     * replace it with @p newValue.
     * 
     * @note This method has architecture dependant implementation
     */
    word_t compareAndSwap(word_t oldValue, word_t newValue);
    
    /**
     * @brief Atomic word get accessor
     */
    word_t get();
    
    /**
     * @brief Atomic word set accessor
     * @param value new value
     */
    void set(word_t value);
    
    /**
     * @brief Atomic OR bits into the word
     * @param mask the mask to be OR-ed
     * 
     * @note This method has architecture dependant implementation
     */
    void setBits(word_t mask);
    
    /**
     * @brief Atomic AND bits into the word
     * @param mask the mask to be AND-ed
     * 
     * @note This method has architecture depemdant implementation
     */
    void clearBits(word_t mask);

  protected:
    word_t  m_word;
};

#endif /* __OOMTKSYS_OATOMIC_H__ */
