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
#include <OOMTK/OAtomic>

#include <types.h>
#include <config.h>
#include <macros.h>

word_t OAtomic::compareAndSwap(word_t oldValue, word_t newValue)
{
  word_t result;
  
  GNU_ASM("lock cmpxchgl %1, %2\n"
  : "=a" (result)
  : "r" (newValue), "m"(m_word), "0" (oldValue)
  : "memory", "cc"
	 );
  
  return result;
}

void OAtomic::setBits(word_t mask)
{
  GNU_ASM("lock orl %[mask], %[word]\n"
  : [word] "+m" (m_word)
  : [mask] "r" (mask)
  : "cc");
}

void OAtomic::clearBits(word_t mask)
{
  GNU_ASM("lock andl %[mask], %[word]\n"
  : [word] "+m" (m_word)
  : [mask] "r" (mask)
  : "cc");
}
