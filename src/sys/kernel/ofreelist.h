#ifndef __OOMTKSYS_OFREELIST_H__
#define __OOMTKSYS_OFREELIST_H__
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
/** @file ofreelist.h
 * @brief Free lists
 */

#include <OOMTK/OAtomicPtr>

class OFreeListItem;

/**
 * @brief Free list class
 */
class OFreeList
{
  public:
    OFreeList();
    ~OFreeList();
    
    OFreeListItem * alloc();
    
    void insert(OFreeListItem * element);

  protected:
    OAtomicPtr<OFreeListItem> m_next;
};

/**
 * @brief Free list item class
 */
class OFreeListItem
{
  public:
    OFreeListItem();
    ~OFreeListItem();
    
  protected:
    OFreeListItem * m_next;
    
    friend class OFreeList;
};

#endif /* __OOMTKSYS_OFREELIST_H__ */
