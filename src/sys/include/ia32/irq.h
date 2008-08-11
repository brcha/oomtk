#ifndef __IA32_IRQ_H__
#define __IA32_IRQ_H__
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
/** @file irq.h
 * @brief Interrupt definitions
 */

/**
 * @brief Number of hardware-defined interrupt entries
 * 
 * The maximal number of interrupt vectors is 256. But this number also contains
 * the 32 lines used for hardware trap vectors. That leaves us with 224 free interrupt
 * vector entries.
 */
#define NUM_IRQ     224

/// @brief Number of hardware-defined trap entries
#define NUM_TRAP    32

/// @brief Number of hardware-defined vector entries (interrupts + traps)
#define NUM_VECTOR  (NUM_IRQ + NUM_TRAP)

#ifndef __ASSEMBLER__

#include <types.h>
#include <cstddef>

/**
 * @brief Number type used for IRQ entries
 * 
 * There are 256 harware interrupts. But, they can be on the pin X on ISA, PCI, or some other bus's
 * interrupt controller and can be mapped to any global IRQ no. It can also be remapped on some other
 * global interrupt entry. Therefore, this type is a bit-field holding the bus identifier in the
 * uppermost 8 bits and pin in the low 24 bits (which is probably too much, but machine word is 32bit).
 * 
 * @note This could have been done as a class, but I think that is too much for a simple bitfield...
 */
typedef uint32_t irq_t;

/**
 * @defgroup InterruptBus Interrupt Bus Sources
 * @{
 */
/// @brief Global system IRQ namespace
#define IBUS_GLOBAL     0x00000000
/// @brief ISA interrupt namespace
#define IBUS_ISA        0x01000000
/// @brief PCI interrupt namespace
#define IBUS_PCI        0x02000000
/// @brief LAPIC interrupt namespace
#define IBUS_LAPIC      0x03000000

/// @brief Make an IRQ from @p bus and @p pin
#define IRQ(bus,pin)    ((bus) | (pin))
/// @brief Get the pin from @p irq
#define IRQ_PIN(irq)    ((irq) & 0x00ffffff)
/// @brief Get the bus from @p irq
#define IRQ_BUS(irq)    ((irq) & 0xff000000)
/** @} */

#include <ia32/registers.h>
#include <ia32/EFLAGS.h>

namespace IRQ
{
  /**
   * @brief Disable interrupts on current CPU
   * @returns the flags register
   */
  inline flags_t locallyDisableInterrupts()
  {
    flags_t flags;
    GNU_ASM("pushfl;\n"
            "cli;\n"
            "popl %0\n"
      : "=g" (flags)
      : /* no input */
           );
    return flags;
  }
  
  /**
   * @brief Enable interrupts on current CPU
   * @param oldFlags flags before interrupts were disables
   */
  inline void locallyEnableInterrupts(flags_t oldFlags)
  {
    GNU_ASM("pushl %0;\n"
            "popfl\n"
      : /* no output */
      : "g" (oldFlags)
      : "memory", "cc"
           );
  }
  
  /**
   * @brief Check if interrupts are enabled on current CPU
   * @returns true if interrupts are enabled
   */
  inline bool localInterruptsEnabled()
  {
    uint32_t flags;
    GNU_ASM("pushfl;\n"
            "popl %0\n"
      : "=g" (flags)
      : /* no input */
           );
    return (flags & EFLAGS_IF) ? true : false;
  }
};
#endif /* __ASSEMBLER__ */

#endif /* __IA32_IRQ_H__ */
