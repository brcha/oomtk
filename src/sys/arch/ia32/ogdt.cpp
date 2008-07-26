/*
 *  Copyright (C) 2006-2007 by Filip Brcic <brcha@gna.org>
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
/** \file
 * \brief Global Descriptor Table
 */

#include "ogdt.h"

#include <ia32/selectors.h>

#include "otss.h"

OGDT * OGDT::instance()
{
  static OGDT instance = OGDT();

  return &instance;
};

OGDT::OGDT()
{
};

void OGDT::initialize()
{
  for (size_t i = 0; i < MAX_NCPU; i++)
  {
    // Null segment = 0
    m_gdt[i][seg_NULL].value = 0;
  
    // TSS segment - setup only the bits, limit and base will be setup later
    setupLimitBase(i, seg_TSS, sizeof(OTSS::TSS_t), KVTOL((kva_t) &(OTSS::instance()->m_tss[i])));
    m_gdt[i][seg_TSS].bits.type        = TSS32avl; // 32bit TSS (available)
    m_gdt[i][seg_TSS].bits.system      = 0; // system segment
    m_gdt[i][seg_TSS].bits.dpl         = 0; // ring 0
    m_gdt[i][seg_TSS].bits.present     = 1; // present
    m_gdt[i][seg_TSS].bits.available   = 0;
    m_gdt[i][seg_TSS].bits.l           = 0; // reserved in regular IA-32 mode
    m_gdt[i][seg_TSS].bits.opSize      = 0;
    m_gdt[i][seg_TSS].bits.granularity = 0; // byte granularity
  
    // Kernel code segment
    // based at 0, size = 4 GB
    setupLimitBase(i, seg_KernelCode, 0xffffffff, 0);
    m_gdt[i][seg_KernelCode].bits.type         = Code | Readable | Accessed;
    m_gdt[i][seg_KernelCode].bits.system       = 1; // code segment
    m_gdt[i][seg_KernelCode].bits.dpl          = 0; // ring 0
    m_gdt[i][seg_KernelCode].bits.present      = 1;
    m_gdt[i][seg_KernelCode].bits.available    = 0;
    m_gdt[i][seg_KernelCode].bits.l            = 0; // reserved
    m_gdt[i][seg_KernelCode].bits.opSize       = 1; // 32bit
    m_gdt[i][seg_KernelCode].bits.granularity  = 1; // 4k granularity
  
    // Kernel data segment
    // based at 0, size = 4 GB
    setupLimitBase(i, seg_KernelData, 0xffffffff, 0);
    m_gdt[i][seg_KernelData].bits.type         = Data | Writable | Accessed;
    m_gdt[i][seg_KernelData].bits.system       = 1;
    m_gdt[i][seg_KernelData].bits.dpl          = 0; // ring 0
    m_gdt[i][seg_KernelData].bits.present      = 1;
    m_gdt[i][seg_KernelData].bits.available    = 0;
    m_gdt[i][seg_KernelData].bits.l            = 0; // reserved
    m_gdt[i][seg_KernelData].bits.opSize       = 1; // 32bit
    m_gdt[i][seg_KernelData].bits.granularity  = 1; // 4k granularity
  
    // User code segment
    // based at 0, size = 4 GB
    setupLimitBase(i, seg_UserCode, 0xffffffff, 0);
    m_gdt[i][seg_UserCode].bits.type           = Code | Readable | Accessed;
    m_gdt[i][seg_UserCode].bits.system         = 1;
    m_gdt[i][seg_UserCode].bits.dpl            = 3; // ring 3
    m_gdt[i][seg_UserCode].bits.present        = 1;
    m_gdt[i][seg_UserCode].bits.available      = 0;
    m_gdt[i][seg_UserCode].bits.l              = 0;
    m_gdt[i][seg_UserCode].bits.opSize         = 1; // 32bit
    m_gdt[i][seg_UserCode].bits.granularity    = 1; // 4k granularity
  
    // User data segment
    // based at 0, size = 4 GB
    setupLimitBase(i, seg_UserData, 0xffffffff, 0);
    m_gdt[i][seg_UserData].bits.type           = Data | Writable | Accessed;
    m_gdt[i][seg_UserData].bits.system         = 1;
    m_gdt[i][seg_UserData].bits.dpl            = 3; // ring 3
    m_gdt[i][seg_UserData].bits.present        = 1;
    m_gdt[i][seg_UserData].bits.available      = 0;
    m_gdt[i][seg_UserData].bits.l              = 0;
    m_gdt[i][seg_UserData].bits.opSize         = 1; // 32bit
    m_gdt[i][seg_UserData].bits.granularity    = 1; // 4k granularity
  
    // User thread-local storage
    // base is variable, but size = 4 GB
    setupLimitBase(i, seg_UserTLS, 0xffffffff, 0);
    m_gdt[i][seg_UserTLS].bits.type          = Data | Writable | Accessed;
    m_gdt[i][seg_UserTLS].bits.system        = 1;
    m_gdt[i][seg_UserTLS].bits.dpl           = 3; // ring 3
    m_gdt[i][seg_UserTLS].bits.present       = 1;
    m_gdt[i][seg_UserTLS].bits.available     = 0;
    m_gdt[i][seg_UserTLS].bits.l             = 0;
    m_gdt[i][seg_UserTLS].bits.opSize        = 1; // 32bit
    m_gdt[i][seg_UserTLS].bits.granularity   = 1; // 4k granularity
  } /* for (i = 1; i < MAX_NCPU; i++) */

//   // Now load the GDTR
//   GNU_ASM(
//       "   lgdt  %[gdtr]       \n" // Load gdtr (linear address)
//       "   movl  %[kds], %%eax \n" // Reload data segments with selKernelDS()
//       "   movw  %%ax, %%es    \n"
//       "   movw  %%ax, %%ds    \n"
//       "   movw  %%ax, %%ss    \n"
//       "   ljmp  %[kcs], $1f   \n" // Reload %eip with selKernelCS()
//       "1: xorl  %%eax, %%eax  \n"
//       "   lldt  %%ax          \n" // Null the LDT
//   : /* no output */
//   : [gdtr] "m" (myGdtr), [kcs] "i" (sel_KernelCS), [kds] "i" (sel_KernelDS)
//   : "memory", "ax"
//          );
};

void OGDT::load()
{
  // to be done...
}
