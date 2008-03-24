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
/** \file init.cpp
 * \brief Kernel initialization
 */

#include <version.h>

#include <typeinfo>

#include <stdio.h>
#include <VgaConsole.h>
// #include <KernelOutputStream.h>

#include <CPUID.h>

#include <Paging.h>
#include <PagingLegacy.h>
#include <PagingPAE.h>

#include <GDT.h>
#include <IDT.h>

#include <fatal.h>

// #include <cxxabi.h>

#include <typeinfo>

using namespace std;

#ifdef __cplusplus
extern "C" {
    /// \brief Multiboot signature (loaded from start.S)
    u32_t multibootSignature;
    /// \brief Multiboot info (loaded from start.S)
    u32_t multibootInfo;
    /// \brief THE C++ Startup Code
    void systemStartup();
    /// \brief The Assembly Halt function (from start.S)
    extern void halt();
}
#endif

void systemStartup()
{
  // Static constructed console
  VgaConsole * vga = VgaConsole::instance();

//   KernelOutputStream<char> kcout = KernelOutputStream<char>(ANSI_NORMAL);
//   KernelOutputStream<char> kcerr = KernelOutputStream<char>(ANSI_FG_RED);
//   KernelOutputStream<char> kclog = KernelOutputStream<char>(ANSI_FG_WHITE);

//   basic_ostream<char> & cout = kcout;
//   basic_ostream<char> & cerr = kcerr;
//   basic_ostream<char> & clog = kclog;

  vga->clear();
  // Say hello to make sure we're really in the higher half...

  Console::stdout = vga;
  Console::stderr = vga;

  // Colored Hello World :)
  printf(ANSI_FG_WHITE "OOMTK version %s%s-r%s%s (built %s%s%s by %s)\n" ANSI_NORMAL,
	 ANSI_FG_CYAN, MKSTR(OOMTK_VERSION), VersionInfo::RevisionNo, ANSI_FG_WHITE,
	 ANSI_FG_CYAN, VersionInfo::BuildDate, ANSI_FG_WHITE,
	 /*              ANSI_FG_CYAN MKSTR(OOMTK_VERSION) ANSI_FG_WHITE,
              ANSI_FG_CYAN MKSTR(OOMTK_BUILD_DATE) ANSI_FG_WHITE,
	 */
              ANSI_FG_CYAN MKSTR(OOMTK_BUILD_USER) ANSI_FG_WHITE
        );

  CPUID * cpuid = CPUID::instance();

  cpuid->identify();

  // Setup the paging
  Paging * paging;
  if (cpuid->has(CPUID::PAE))
    paging = PagingPAE::instance();
  else
    paging = PagingLegacy::instance();
  paging->setup();

  // Setup the new GDT
  GDT * gdt = GDT::instance();
  gdt->setup();

  printf("Typeid(gdt).name() = %s\n", typeid(gdt).name());

  // Setup the new IDT
  IDT * idt = IDT::instance();
  idt->setup();

  // Call the constructors
//   callCtors();

  for (;;);

  // Call destructors
//   callDtors();
}

