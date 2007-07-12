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
 * \brief Global Descriptor Table
 */

#include "GDT.h"
#include "Selectors.h"

#include <stdio.h>
#include <ansi.h>

/**
 * \brief Enable debugging messages or not
 */
#define DEBUG_GDT DEBUG_ENABLE

/**
 * \brief Get one instance of this class
 * \returns the instance
 */
GDT * GDT::instance()
{
  static GDT instance = GDT();

  return &instance;
};

DEFINE_PER_CPU(uint64_t[seg_Count], myGdt);
DEFINE_PER_CPU(uint64_t, myGdtr);

/**
 * \brief Protected constructor
 */
GDT::GDT()
{
  gdt = reinterpret_cast<SegmentDescriptor_t*>(&myGdt);
  gdtr = reinterpret_cast<GDTR_t*>(&myGdtr);

  gdtr->bits.limit   = sizeof(uint64_t) * seg_Count;
  gdtr->bits.address = reinterpret_cast<uint32_t>(&myGdt[0]);
  gdtr->bits.nothing = 0; // not used for anything, but just in case init to 0
};

/**
 * \brief Setup the Global Descriptor Table
 */
void GDT::setup()
{
  DEBUG_GDT
      printf("Loading GDT ... ");

  // Null segment = 0
  gdt[seg_NULL].value = 0;

  // Kernel code segment
  // based at 0, size = 4 GB
  setupLimitBase(seg_KernelCS, 0xffffffff, 0);
  gdt[seg_KernelCS].bits.type         = Code | Readable | Accessed;
  gdt[seg_KernelCS].bits.system       = 1; // code segment
  gdt[seg_KernelCS].bits.dpl          = 0; // ring 0
  gdt[seg_KernelCS].bits.present      = 1;
  gdt[seg_KernelCS].bits.available    = 0;
  gdt[seg_KernelCS].bits.l            = 0; // reserved
  gdt[seg_KernelCS].bits.opSize       = 1; // 32bit
  gdt[seg_KernelCS].bits.granularity  = 1; // 4k granularity

  // Kernel data segment
  // based at 0, size = 4 GB
  setupLimitBase(seg_KernelDS, 0xffffffff, 0);
  gdt[seg_KernelDS].bits.type         = Data | Writable | Accessed;
  gdt[seg_KernelDS].bits.system       = 1;
  gdt[seg_KernelDS].bits.dpl          = 0; // ring 0
  gdt[seg_KernelDS].bits.present      = 1;
  gdt[seg_KernelDS].bits.available    = 0;
  gdt[seg_KernelDS].bits.l            = 0; // reserved
  gdt[seg_KernelDS].bits.opSize       = 1; // 32bit
  gdt[seg_KernelDS].bits.granularity  = 1; // 4k granularity

  // User code segment
  // based at 0, size = 4 GB
  setupLimitBase(seg_UserCS, 0xffffffff, 0);
  gdt[seg_UserCS].bits.type           = Code | Readable | Accessed;
  gdt[seg_UserCS].bits.system         = 1;
  gdt[seg_UserCS].bits.dpl            = 3; // ring 3
  gdt[seg_UserCS].bits.present        = 1;
  gdt[seg_UserCS].bits.available      = 0;
  gdt[seg_UserCS].bits.l              = 0;
  gdt[seg_UserCS].bits.opSize         = 1; // 32bit
  gdt[seg_UserCS].bits.granularity    = 1; // 4k granularity

  // User data segment
  // based at 0, size = 4 GB
  setupLimitBase(seg_UserDS, 0xffffffff, 0);
  gdt[seg_UserDS].bits.type           = Data | Writable | Accessed;
  gdt[seg_UserDS].bits.system         = 1;
  gdt[seg_UserDS].bits.dpl            = 3; // ring 3
  gdt[seg_UserDS].bits.present        = 1;
  gdt[seg_UserDS].bits.available      = 0;
  gdt[seg_UserDS].bits.l              = 0;
  gdt[seg_UserDS].bits.opSize         = 1; // 32bit
  gdt[seg_UserDS].bits.granularity    = 1; // 4k granularity

  // User thread-local storage
  // base is variable, but size = 4 GB
  setupLimitBase(seg_UserTLS, 0xffffffff, 0);
  gdt[seg_UserTLS].bits.type          = Data | Writable | Accessed;
  gdt[seg_UserTLS].bits.system        = 1;
  gdt[seg_UserTLS].bits.dpl           = 3; // ring 3
  gdt[seg_UserTLS].bits.present       = 1;
  gdt[seg_UserTLS].bits.available     = 0;
  gdt[seg_UserTLS].bits.l             = 0;
  gdt[seg_UserTLS].bits.opSize        = 1; // 32bit
  gdt[seg_UserTLS].bits.granularity   = 1; // 4k granularity

  // TSS segment - setup only the bits, limit and base will be setup later
  setupLimitBase(seg_TSS, 104, 0);
  gdt[seg_TSS].bits.type        = TSS32avl; // 32bit TSS (available)
  gdt[seg_TSS].bits.system      = 0; // system segment
  gdt[seg_TSS].bits.dpl         = 0; // ring 0
  gdt[seg_TSS].bits.present     = 1; // present
  gdt[seg_TSS].bits.available   = 0;
  gdt[seg_TSS].bits.l           = 0; // reserved in regular IA-32 mode
  gdt[seg_TSS].bits.opSize      = 0;
  gdt[seg_TSS].bits.granularity = 0; // byte granularity

  // Now load the GDTR
  GNU_ASM(
      "   lgdt  %[gdtr]       \n" // Load gdtr (linear address)
      "   movl  %[kds], %%eax \n" // Reload data segments with selKernelDS()
      "   movw  %%ax, %%es    \n"
      "   movw  %%ax, %%ds    \n"
      "   movw  %%ax, %%ss    \n"
      "   ljmp  %[kcs], $1f   \n" // Reload %eip with selKernelCS()
      "1: xorl  %%eax, %%eax  \n"
      "   lldt  %%ax          \n" // Null the LDT
  : /* no output */
  : [gdtr] "m" (myGdtr), [kcs] "i" (sel_KernelCS), [kds] "i" (sel_KernelDS)
  : "memory", "ax"
         );

  DEBUG_GDT
      printf(ANSI_FG_GREEN "[ok]\n" ANSI_NORMAL);
};
