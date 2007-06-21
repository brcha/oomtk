#ifndef __IA32_CONTEXT_H__
#define __IA32_CONTEXT_H__
/*
 *  Copyright (C) 2007 by Filip Brčić <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file Context.h
 * \brief IA32 Process Context structure
 */
/*
 * $Id: $
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
};

#endif /* __IA32_CONTEXT_H__ */
