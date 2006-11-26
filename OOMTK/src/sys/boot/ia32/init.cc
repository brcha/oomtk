/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/*
 * $Id: $
 */
/** @file
 * @brief Kernel initialization
 */

#include "ctors.h"
#include <stdio.h>
#include <VgaConsole.h>

#ifdef __cplusplus
extern "C" void init(void * mbd);
#endif

// WARNING: mbd is a *physical* address. According to the Multiboot
// spec, there are no guarantees about its location, so we can't just
// assume it's in the first 4MB and already mapped to the higher-half.
// TODO: Write some code to map the Multiboot info structure somewhere
// in the kernel's address space.
void init(void * mbd)
{
  // Call constructors
  callCtors();

  // Static constructed console
  VgaConsole vga = VgaConsole();

  // Say hello to make sure we're really in the higher half...

  vga.clear();
  Console::stdout = &vga;
  printf("OOMTK version %s build %s (built %s by %s)\n",
              OOMTK_VERSION, OOMTK_BUILD, OOMTK_BUILD_DATE, OOMTK_BUILD_USER);

  for (;;);

  // Call destructors
  callDtors();
}
