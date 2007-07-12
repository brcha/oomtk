/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK (http://launchpad.net/oomtk)
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
/** \file
 * \brief Abstract Port I/O API
 *
 * NOTE: This class must have architecture specific implementations!
 */

#include "PortIO.h"

// Only the delayed versions are implemented here
u8_t PortIO::in8_d(word_t port)
{
  u8_t tmp = in8(port);
  iodelay();
  return tmp;
}

u16_t PortIO::in16_d(word_t port)
{
  u16_t tmp = in16(port);
  iodelay();
  return tmp;
}

u32_t PortIO::in32_d(word_t port)
{
  u32_t tmp = in32(port);
  iodelay();
  return tmp;
}

void PortIO::out8_d(word_t port, u8_t value)
{
  out8(port,value);
  iodelay();
}

void PortIO::out16_d(word_t port, u16_t value)
{
  out16(port,value);
  iodelay();
}

void PortIO::out32_d(word_t port, u32_t value)
{
  out32(port,value);
  iodelay();
}
