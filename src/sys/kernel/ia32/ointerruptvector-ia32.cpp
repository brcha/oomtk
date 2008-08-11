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
#include <OOMTK/OInterruptVector>

#include <fatal.h>
#include <cassert>

OInterruptVector * OInterruptVector::mapInterrupt(irq_t irq)
{
  switch (IRQ_BUS(irq))
  {
    case IBUS_ISA:
    {
      // TODO: IntSrcOverride isovr;
      // if (acpi_map_interrupt(irq, &isovr))
      //    irq = isovr.globalSystemInterrupt;
      // else
      irq = IRQ(IBUS_GLOBAL, IRQ_PIN(irq));
      return m_irqVector[irq];
    }
    case IBUS_GLOBAL:
      return m_irqVector[irq];
    case IBUS_LAPIC:
      // LAPIC interrupts are programmed with vector number directly
      return &m_vectorMap[IRQ_PIN(irq)];
    default:
      fatal("Unknown bus type for binding.\n");
  }
}

void OInterruptVector::irqBind(irq_t irq, OInterruptVector::VectorMode mode,
                               OInterruptVector::VectorLevel level, OInterruptVector::handler_t handler)
{
  OInterruptVector * vector;
  
  switch (IRQ_BUS(irq))
  {
    case IBUS_ISA:
    {
      mode = OInterruptVector::vm_Edge;
      level = OInterruptVector::vl_ActiveHigh;
      
      // TODO: IntSrcOverride isovr;
      // if (acpi_map_interrupt(irq, &isovr))
      // {
      //    irq = isovr.globalSystemInterrupt;
      //    if ((isovr.flags & MPS_INTI_TRIGGER) == MPS_INTI_TRIGGER_LEVEL)
      //        mode = OInterruptVector::vm_Level;
      //    if ((isovr.flags & MPS_INTI_POLARITY) == MPS_INTI_POLARITY_ACTIVELOW)
      //        level = OInterruptVector::vl_ActiveLow;
      // } else
      irq = IRQ(IBUS_GLOBAL, IRQ_PIN(irq));
      
      vector = m_irqVector[irq];
      break;
    }
    case IBUS_LAPIC:
    {
      vector = &m_vectorMap[IRQ_PIN(irq)];
      break;
    }
    default:
      fatal("Unknown bus type for binding.\n");
  }
  
  assert(vector);
  
  vector_hold_info_t hi = vector->grab();
  vector->m_count = 0;
  vector->m_handler = handler;
  vector->m_mode = mode;
  vector->m_level = level;
  vector->m_controller->setup(vector);
  vector->release(hi);
};
