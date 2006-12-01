/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** @file
 * @brief Kernel initialization
 */

#include <stdio.h>
#include <VgaConsole.h>
#include <KernelOutputStream.h>

#include <CPUID.h>

#include <SegmentDescriptor.h>

#include <Paging.h>
#include <PagingLegacy.h>
#include <PagingPAE.h>

#include <fatal.h>

#include <c++abi.h>

using namespace std;

#ifdef __cplusplus
extern "C" {
    /// Multiboot signature (loaded from start.S)
    u32_t multibootSignature;
    /// Multiboot info (loaded from start.S)
    u32_t multibootInfo;
    /// THE C++ Startup Code
    void systemStartup();
    /// The Assembly Halt function (from start.S)
    extern void halt();
}
#endif

void systemStartup()
{
  // Static constructed console
  VgaConsole * vga = VgaConsole::instance();

  KernelOutputStream<char> kcout = KernelOutputStream<char>(ANSI_NORMAL);
  KernelOutputStream<char> kcerr = KernelOutputStream<char>(ANSI_FG_RED);
  KernelOutputStream<char> kclog = KernelOutputStream<char>(ANSI_FG_WHITE);

  basic_ostream<char> & cout = kcout;
  basic_ostream<char> & cerr = kcerr;
  basic_ostream<char> & clog = kclog;

  vga->clear();
  // Say hello to make sure we're really in the higher half...

  Console::stdout = vga;
  Console::stderr = vga;

  // Colored Hello World :)
  printf(ANSI_FG_WHITE "OOMTK version %s build %s (built %s by %s)\n" ANSI_NORMAL,
              ANSI_FG_CYAN OOMTK_VERSION ANSI_FG_WHITE,
              ANSI_FG_CYAN OOMTK_BUILD ANSI_FG_WHITE,
              ANSI_FG_CYAN OOMTK_BUILD_DATE ANSI_FG_WHITE,
              ANSI_FG_CYAN OOMTK_BUILD_USER ANSI_FG_WHITE
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

  // Try to setup new GDT
  printf("Before new GDT\n");
//   SetupGDT();
  printf("After new GDT :)\n");

  // Call the constructors
//   callCtors();

  for (;;);

  // Call destructors
//   callDtors();
}

