#ifndef __GENERIC_CONTEXT_INTERFACE_H__
#define __GENERIC_CONTEXT_INTERFACE_H__
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
/** \file ContextInterface.h
 * \brief Description
 */

#include <types.h>

class ContextInterface
{
  public:
    virtual inline void setSP(addr_t sp) = 0;
    virtual inline void setPC(addr_t pc) = 0;
};

#endif /* __GENERIC_CONTEXT_INTERFACE_H__ */
