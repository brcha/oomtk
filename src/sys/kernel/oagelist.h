#ifndef __OOMTKSYS_OAGELIST_H__
#define __OOMTKSYS_OAGELIST_H__
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
/** @file oagelist.h
 * @brief Age list class
 */
#include <OOMTK/OLinkedList>

#include <types.h>
#include <cstddef>

class OAgeList;

/**
 * @brief Everything in the age list should inherit ageable class
 */
class OAgeable : public OLinkedList
{
  public:
    OAgeable();
    ~OAgeable();
    
  protected:
    OAgeList    * m_list;
    
    friend class  OAgeList;
};

/**
 * @brief Age list class
 * 
 * Age list is a list of ageable objects. Objects can be removed from any point in
 * the list, but can only be added at the front and at the back of the list.
 */
class OAgeList
{
  public:
    OAgeList();
    ~OAgeList();
    
    /**
     * @brief Check if count has dropped below the low water
     * @returns true if count has dropped below the low water
     */
    bool underLowWater();
    
    /**
     * @brief Check if count is below high water
     * @returns true if count is below high water
     */
    bool underHighWater();
    
    /**
     * @brief Check if the list is empty
     * @returns true if list is empty
     */
    bool isEmpty();
    
    /**
     * @brief Check is the list has more elements than @p count
     * @returns true if list has more elements than @p count
     */
    bool hasMoreThan(size_t count);
    
    /**
     * @brief Find the oldest item on the list
     * @returns the oldest item on the list
     */
    OAgeable * oldest();
    
    /**
     * @brief Remove the item from the age list.
     * @param element to be removed
     */
    void remove(OAgeable * element);
    
    /**
     * @brief Check if @p element is oldest in the list
     * @returns true if @p element if oldest in the list
     */
    bool isOldest(OAgeable * element);
    
    /**
     * @brief Remove the oldest item from the list
     * @returns the oldest item on the list
     */
    OAgeable * removeOldest();
    
    /**
     * @brief Add an item to the front of the list
     * @param newElement item to be added
     * 
     * @note The @p newElement will become the newest on the list
     */
    void addFront(OAgeable * newElement);
    
    /**
     * @brief Add an item to the back of the list
     * @param newElement item to be added
     * 
     * @note The @p newElement will become the oldest on the list
     */
    void addBack(OAgeable * newElement);

  protected:
    OAgeable    m_list;       ///< @brief Root of the age list
    size_t      m_count;      ///< @brief Number of elements in the age list
    bool        m_needsFill;  ///< @brief count fell below low water but has not reached high water yet
    size_t      m_lowWater;   ///< @brief Low water: if cound is below this, list should be filled to high water
    size_t      m_highWater;  ///< @brief High water
};

#endif
