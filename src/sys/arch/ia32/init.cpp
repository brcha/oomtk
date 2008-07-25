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

#include <OOMTK/OVgaConsole>

extern "C" {
  uint32_t multibootSignature;  ///< @brief Multiboot signature
  uint32_t multibootInfo;       ///< @brief Multiboot information pointer
  
  // Locations from ldscript.S:
  extern uint32_t _start[];
  extern uint32_t _etext[];
  extern uint32_t __syscall_page[];
  extern uint32_t __esyscall_page[];
  extern uint32_t _pagedata[];
  extern uint32_t _end[];
  extern uint32_t _bss_end[];
  extern uint32_t __begin_maps[];
  extern uint32_t __end_maps[];
  extern uint32_t cpu0_kstack_lo[];
  extern uint32_t cpu0_kstack_hi[];
}

extern "C" void arch_init(void);

/**
 * @brief Perform architecture-specific initialization
 * 
 * arch_init() performs the low level initialization of the system before the real kernel starts
 * to work. It does the following tasks:
 * 
 * <ul>
 * <li>Initialize the boot console system that is used to display early bootup information. When the
 * kernel ends booting, the console will be shut down and released for use by user processes.</li>
 * <li>Initialize the physical memory so that memory ranges could be allocated.</li>
 * <li>Reserve memory space for kernel and page tables</li>
 * <li>Setup GDT, LDT, TSS and IDT tables.</li>
 * </ul>
 */
void arch_init(void)
{
  /* Initialize the console
   */
  OVgaConsole * vga = OVgaConsole::instance();
  vga->initialize();
  
  for (;;);
}