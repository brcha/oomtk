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
/** \file InterruptHandler.h
 * \brief C Interrupt handling routine
 */
#ifndef __IA32_INTERRUPT_HANDLER_H__
#define __IA32_INTERRUPT_HANDLER_H__

#include <types.h>

extern "C" {
  void interruptHandler(addr_t saveArea);
};

#endif /* __IA32_INTERRUPT_HANDLER_H__ */
