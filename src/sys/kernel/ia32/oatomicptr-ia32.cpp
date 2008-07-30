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
#include "oatomicptr-ia32.h"

#include <macros.h>
#include <types.h>

void * OOMTK::IA32::compareAndSwapPtr(void **ap, void * oldValue, void * newValue)
{
  void * result;
  
  GNU_ASM("lock cmpxchgl %1, %2"
  : "=a" (result)
  : "r" (newValue), "m" (*(word_t*)(ap)), "0" (oldValue)
  : "memory", "cc");
  
  return result;
}