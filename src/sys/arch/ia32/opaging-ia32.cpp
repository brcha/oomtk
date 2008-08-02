/*
 *  Copyright (C) 2006-2008 by Filip Brcic <brcha@gna.org>
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
/** @file
 * @brief Paging support
 *
 * @todo Is this file IA32 specific or generic???
 */
#include "opaging-ia32.h"
#include "opaginglegacy.h"
#include "opagingpae.h"

#include <OOMTK/OCPU>

#include <cstdio>

#include <ia32/pagesize.h>

extern "C" {
  bool UsingPAE;
}

OPagingIA32 * OPagingIA32::instance()
{
  if (UsingPAE)
    return OPagingPAE::instance();
  else
    return OPagingLegacy::instance();
}
// 
// Low-level get/set operators

uint32_t OPagingIA32::cr0()
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

void OPagingIA32::cr0(uint32_t value)
{
  GNU_ASM(
	  "   movl  %[value], %%edx   \n"
      "   mov   %%edx, %%cr0      \n" // write the register
  : /* no output */
  : [value] "m" (value)
  : "edx"
	 );
};

uint32_t OPagingIA32::cr3()
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

void OPagingIA32::cr3(uint32_t value)
{
  GNU_ASM(
	  "   movl  %[value], %%edx   \n"
      "   mov   %%edx, %%cr3      \n" // write the register
  : /* no output */
  : [value] "m" (value)
  : "edx"
	 );
};

uint32_t OPagingIA32::cr4()
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

void OPagingIA32::cr4(uint32_t value)
{
  GNU_ASM(
	  "   movl  %[value], %%edx   \n"
      "   mov   %%edx, %%cr4      \n" // write the register
  : /* no output */
  : [value] "m" (value)
  : "edx"
	 );
};
