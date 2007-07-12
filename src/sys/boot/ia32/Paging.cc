/*
 *  Copyright (C) 2006-2007 by Filip Brcic <brcha@users.sourceforge.net>
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
 * \brief Paging support
 *
 * \todo Is this file IA32 specific or generic???
 */
#include "Paging.h"

#include <fatal.h>
#include <ansi.h>

// Low-level get/set operators

/**
 * \brief Get the CR0 register
 * \returns value of the CR0 register
 */
uint32_t Paging::cr0()
{
  uint32_t value;

  GNU_ASM(
      "   mov   %%cr0, %%edx     \n" // read the register
      "   movl  %%edx, %[value]  \n"
  : [value] "=m" (value)
  : /* no input */
  : "edx", "memory"
         );

  return value;
};

/**
 * \brief Set the CR0 register
 * \param value new value for the CR0 register
 *
 * \warning This method enables paging (if PG bit is set), so take care how you use it
 */
void Paging::cr0(uint32_t value)
{
  GNU_ASM(
      "   movl  %[value], %%edx   \n"
      "   mov   %%edx, %%cr0      \n" // write the register
  : /* no output */
  : [value] "m" (value)
  : "edx"
         );
};

/**
 * \brief Get the CR3 register
 * \returns value of the CR3 register
 */
uint32_t Paging::cr3()
{
  uint32_t value;

  GNU_ASM(
      "   mov   %%cr3, %%edx     \n" // read the register
      "   movl  %%edx, %[value]  \n"
  : [value] "=m" (value)
  : /* no input */
  : "edx", "memory"
         );

  return value;
};

/**
 * \brief Set the CR3 register
 * \param value new value for CR3 register
 */
void Paging::cr3(uint32_t value)
{
  GNU_ASM(
      "   movl  %[value], %%edx   \n"
      "   mov   %%edx, %%cr3      \n" // write the register
  : /* no output */
  : [value] "m" (value)
  : "edx"
         );
};

/**
 * \brief Get the CR4 register
 * \returns the value from CR4
 */
uint32_t Paging::cr4()
{
  uint32_t value;

  GNU_ASM(
      "   mov   %%cr4, %%edx     \n" // read the register
      "   movl  %%edx, %[value]  \n"
  : [value] "=m" (value)
  : /* no input */
  : "edx", "memory"
         );

  return value;
};

/**
 * \brief Set the CR4 register
 * \param value new value for CR4 register
 */
void Paging::cr4(uint32_t value)
{
  GNU_ASM(
      "   movl  %[value], %%edx   \n"
      "   mov   %%edx, %%cr4      \n" // write the register
  : /* no output */
  : [value] "m" (value)
  : "edx"
         );
};

