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
#include "olocalapic.h"

OLocalAPIC::OLocalAPIC()
{
}


OLocalAPIC::~OLocalAPIC()
{
}

OLocalAPIC * OLocalAPIC::instance()
{
  static OLocalAPIC _instance = OLocalAPIC();

  return &_instance;
}




bool OLocalAPIC::lapicRequires8259disable() const
{
  return m_lapicRequires8259disable;
}


void OLocalAPIC::setLapicRequires8259disable (bool theValue)
{
  m_lapicRequires8259disable = theValue;
}


kpa_t OLocalAPIC::lapicPA() const
{
  return m_lapicPA;
}


void OLocalAPIC::setLapicPA (const kpa_t& theValue)
{
  m_lapicPA = theValue;
}


kva_t OLocalAPIC::lapicVA() const
{
  return m_lapicVA;
}


void OLocalAPIC::setLapicVA (const kva_t& theValue)
{
  m_lapicVA = theValue;
}

uint32_t OLocalAPIC::readRegister(uint32_t reg)
{
  volatile uint32_t * va_reg = reinterpret_cast<uint32_t *> (m_lapicVA + reg);
  uint32_t val = *va_reg;
  return val;
}

void OLocalAPIC::writeRegister(uint32_t reg, uint32_t value)
{
  volatile uint32_t * va_reg = reinterpret_cast<uint32_t *> (m_lapicVA + reg);
  *va_reg = value;
  
  // Follow up with read from ID register, forcing above write to have observable effect
  (void) readRegister(LAPIC_ID);
}

void OLocalAPIC::eoi()
{
  // Can write anything, but for future compatibility 0 is suggested
  writeRegister(LAPIC_EOI, 0x0);
}
