#ifndef __OOMTKSYS_OLINKEDLIST_H__
#define __OOMTKSYS_OLINKEDLIST_H__
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
/** @file olinkedlist.h
 * @brief Double linked circular list class
 */

/**
 * @brief Circularly double linked list
 * 
 * This is an abstract circular linked list that doesn't implement data. You have to inherit
 * this class to actually have any use of this list.
 */
class OLinkedList
{
  public:
    OLinkedList();
    ~OLinkedList();
    
    /**
     * @brief Check if the list is empty
     * @returns true if the list is empty
     */
    bool isEmpty();
    
    /**
     * @brief Unlink the current list element
     */
    void unlink();
    
    /**
     * @brief Insert an element after this one
     * @param newItem new linked list element to be inserted
     */
    void insertAfter(OLinkedList * newItem);
    
    /**
     * @brief Insert an element before this one
     * @param newItem new linked list element to be inserted
     */
    void insertBefore(OLinkedList * newItem);
    
    /**
     * @returns the next element of the list
     */
    OLinkedList * next();
    
    /**
     * @returns the previous element of the list
     */
    OLinkedList * prev();
    
  protected:
    OLinkedList * m_next;
    OLinkedList * m_prev;

};

#endif /* __OOMTKSYS_OLINKEDLIST_H__ */
