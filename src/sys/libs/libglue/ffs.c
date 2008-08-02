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
#include "ffs.h"
#include <stddef.h>

int ffs(int val)
{
  size_t i;
  
  for (i = 0; i < (sizeof(val) * 8); i++)
  {
    if ((1u << i) & (unsigned int) val)
      return i+1;
  }
  
  return 0;
}

int ffsl(long val)
{
  size_t i;
  
  for (i = 0; i < (sizeof(val) * 8); i++)
  {
    if ((1u << i) & (unsigned long) val)
      return i+1;
  }
  
  return 0;
}

int ffsll(long long val)
{
  size_t i;
  
  for (i = 0; i < (sizeof(val) * 8); i++)
  {
    if ((1llu << i) & (unsigned long long) val)
      return i+1;
  }
  
  return 0;
}