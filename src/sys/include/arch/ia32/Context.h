#ifndef __IA32_CONTEXT_H__
#define __IA32_CONTEXT_H__
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
/** \file Context.h
 * \brief IA32 Process Context structure
 */

#include <ContextInterface.h>

/** Process Context for IA32
 */
class Context : public ContextInterface
{
  public:
    virtual inline void setSP(addr_t sp)
    {
      ESP = (uint32_t) sp;
    };

    virtual inline void setPC(addr_t pc)
    {
      EIP = (uint32_t) pc;
    }

  protected:
    /* v- Saved by pusha -v */
    uint32_t   EDI;
    uint32_t   ESI;
    uint32_t   EBP;
    uint32_t   ExceptAddr;  /* This points to handler's ESP before pusha */
    uint32_t   EBX;
    uint32_t   EDX;
    uint32_t   ECX;
    uint32_t   EAX;
    /* ^- Saved by pusha -^ */
    uint32_t   ExceptNo;    /* <- Saved by interrupt handler entry */
    /* v- Saved by CPU on Interrupt -v */
    uint32_t   Error;
    uint32_t   EIP;
    uint32_t   CS;
    uint32_t   EFLAGS;
    uint32_t   ESP; /* -> Only on context switch (out-of-kernel interrupt) */
    uint32_t   SS;  /* /                                                   */
    /* ^- Saved by CPU on Interrupt -^ */
    /* v- Saved by common interrupt handler routine -v */
    uint32_t   DS;
    uint32_t   ES;
    uint32_t   FS;
    uint32_t   GS;
    /* ^- Saved by common interrupt handler routine -^ */

    OFFSET_FRIENDLY; // declare this class as friendly with offset generator
    friend void interruptHandler(addr_t saveArea); // NOTE: temporary, for testing only
    friend class InterruptManager; // allow the InterruptManager class to go through the context contents
    // TODO: Pluggable interrupt handlers must have some way to access this data... maybe interface?
};

#endif /* __IA32_CONTEXT_H__ */
