#ifndef __OOMTKSYS_OIRQCONTROLLER_H__
#define __OOMTKSYS_OIRQCONTROLLER_H__
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
/** @file oirqcontroller.h
 * @brief IRQ Controller clas
 */

#include <types.h>
#include <cstddef>
#include <macros.h>

#include INC_ARCH(irq.h)

class OInterruptVector;

/**
 * @brief Abstract interface for all interrupt controller chips
 *
 * @note Actual implementation of this class is likely to have intialize() and shutdown() methods
 * (or something similar), but as they are not to be called outside of architecture dependant code,
 * they are not part of this interface.
 */
class OIRQController
{
  public:
    OIRQController() {};
    virtual ~OIRQController() {};

    // NOTE: I first put OInterruptVector& vec to all these methods, but ended up writing
    // "*this" in every call to these methods. Therefore, it is nicer to put pointers
    // here :)
    /**
     * @brief Setup the vector for the interrupt controller
     * @param vec the vector to be setup
     */
    virtual void setup (OInterruptVector * vec) = 0;
    /**
     * @brief Check if interrupt is pending
     * @param vec vector to be checked
     * @returns true if pending
     */
    virtual bool isPending (OInterruptVector * vec) = 0;
    /**
     * @brief Mask the interrupt
     * @param vec interrupt vector to be masked
     */
    virtual void mask (OInterruptVector * vec) = 0;
    /**
     * @brief Unmask the interrupt vector
     * @param vec vector to be unmasked
     */
    virtual void unmask (OInterruptVector * vec) = 0;
    /**
     * @brief Acknoledge receiving the interrupt
     * @param vec interrupt vector that is received and needs to be acknoledged
     */
    virtual void ack (OInterruptVector * vec) = 0;

  protected:
    irq_t m_baseIRQ;      ///< @brief First IRQ on this controller
    irq_t m_numIRQ;       ///< @brief Number of IRQ sources on this controller
    kva_t m_va;           ///< @brief VA for memory-mapped controllers
};

#endif /* __OOMTKSYS_OIRQCONTROLLER_H__ */
