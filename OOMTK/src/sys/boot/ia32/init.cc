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

#include <CPUID.h>

#include <SegmentDescriptor.h>

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
  VgaConsole vga = VgaConsole();

  vga.clear();
  // Say hello to make sure we're really in the higher half...

  Console::stdout = &vga;
  Console::stderr = &vga;
  printf("OOMTK version %s build %s (built %s by %s)\n",
              OOMTK_VERSION, OOMTK_BUILD, OOMTK_BUILD_DATE, OOMTK_BUILD_USER);

  CPUID cpuid = CPUID();

  cpuid.identify();

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
