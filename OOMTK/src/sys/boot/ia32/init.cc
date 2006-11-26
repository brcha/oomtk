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
  // Try to initialize GDT
//   SetupGDT();
  // Initialize Paging
//   halt();

  // Call constructors
//   callCtors();

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

//   clog << "OOMTK version " << ANSI_FG_CYAN OOMTK_VERSION
//        << " build " << ANSI_FG_CYAN OOMTK_BUILD
//        << " (built " << ANSI_FG_CYAN OOMTK_BUILD_DATE
//        << " by " << ANSI_FG_CYAN OOMTK_BUILD_USER << ")" << endl;

  CPUID * cpuid = CPUID::instance();

  cpuid->identify();

  Paging * paging = Paging::instance();

  for (;;);

  // Call destructors
//   callDtors();
}

/** @brief IA32 PAE PTE data structure. */
union IA32_PAE {
  struct {
    u64_t V       :  1;	/**< valid */
    u64_t W       :  1;	/**< writable  */
    u64_t USER    :  1;	/**< user-accessable */
    u64_t PWT     :  1;	/**< page write through */
    u64_t PCD     :  1;	/**< page cache disable */
    u64_t ACC     :  1;	/**< accessed */
    u64_t DIRTY   :  1;	/**< dirty */
    u64_t PGSZ    :  1;	/**< large page */
    u64_t GLBL    :  1;	/**< global mapping */
    u64_t SW0     :  1;	/**< software use */
    u64_t SW1     :  1;	/**< software use */
    u64_t SW2     :  1;	/**< software use */
    u64_t frameno : 40;	/**< frame number */
  } bits;
  /* Declared in IPC-vars.cxx: */

  u64_t value;
};
typedef union IA32_PAE IA32_PAE;

bool UsingPAE = false;

DEFINE_PER_CPU(IA32_PAE[4], KernelPDPT);
