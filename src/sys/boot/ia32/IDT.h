/*
 *  Copyright (C) 2007 by Filip Brčić <brcha@users.sourceforge.net>
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
/** \file IDT.h
 * \brief Description
 */
#ifndef __IA32_IDT_H__
#define __IA32_IDT_H__

#include <types.h>

/**
 * \brief Interrupt descriptor table
 */
class IDT
{
  public:
    /**
     * \brief Singleton instance of this class
     */
    static IDT * instance();

    /**
     * \brief Setup the IDT & IDTR
     */
    void setup();

  protected:
    IDT();

    /**
     * \brief Set the hardware vector
     * \param entry irq entry
     * \param handlerPtr pointer to handler procedure
     * \param user true if this is an user callable interrupt
     */
    void SetHardwareVector(int entry, void (*handlerPtr)(void), bool user = false);

  protected:
    union GateDescriptor_t {
      struct {
        uint32_t  offset_lo : 16; ///< \brief Offset to procedure entry point (lower word)
        uint32_t  selector  : 16; ///< \brief Segment selector for destination code segment
        uint32_t  zero      :  8; ///< \brief 5 undefined fields + 3 zeros (= 8 zeros)
        uint32_t  type      :  4; ///< \brief Gate type (task, interrupt or trap gate)
        uint32_t  system    :  1; ///< \brief System or user gate
        uint32_t  dpl       :  2; ///< \brief Descriptor Privilege Level
        uint32_t  present   :  1; ///< \brief Segment Present flag
        uint32_t  offset_hi : 16; ///< \brief Offset to procedure entry point (higher word)
      } bits;                     ///< \brief Bits of gate descriptor
      uint64_t    value;          ///< \brief Gate descriptor 64bit value
    };

    GateDescriptor_t * IdtTable;
    uint32_t *         IdtDescriptor;
};

#endif /* __IA32_IDT_H__ */
