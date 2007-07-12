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
 * \brief Port I/O API - ia32 implementation
 */
#include "PortIO.h"

/* Linux, L4, Coyotos uses this for iodelay, so I guess, I can too */
void PortIO::iodelay()
{
  asm volatile (
      "outb %al,$0x80\n\t"
      "outb %al,$0x80 \n\t"
  );
}

u8_t PortIO::in8(word_t port)
{
  u8_t tmp;
  asm volatile ("inb %w1, %b0" : "=a"(tmp) : "Nd"(port));
  return tmp;
}

u16_t PortIO::in16(word_t port)
{
  u16_t tmp;
  asm volatile ("inw %w1, %w0" : "=a"(tmp) : "Nd"(port));
  return tmp;
}

u32_t PortIO::in32(word_t port)
{
  u32_t tmp;
  asm volatile ("in %w1, %0" : "=a"(tmp) : "Nd"(port));
  return tmp;
}

void PortIO::out8(word_t port, u8_t value)
{
  asm volatile ("outb %b0, %w1" : : "a"(value), "Nd"(port));
}

void PortIO::out16(word_t port, u16_t value)
{
  asm volatile ("outw %w0, %w1" : : "a"(value), "Nd"(port));
}

void PortIO::out32(word_t port, u32_t value)
{
  asm volatile ("out %0, %w1" : : "a"(value), "Nd"(port));
}
