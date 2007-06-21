#ifndef __GENERIC_CONTEXT_INTERFACE_H__
#define __GENERIC_CONTEXT_INTERFACE_H__
/*
 *  Copyright (C) 2007 by Filip Brčić <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file ContextInterface.h
 * \brief Description
 */
/*
 * $Id: $
 */

#include <types.h>

class ContextInterface
{
  public:
    virtual inline void setSP(addr_t sp) = 0;
    virtual inline void setPC(addr_t pc) = 0;
};

#endif /* __GENERIC_CONTEXT_INTERFACE_H__ */
