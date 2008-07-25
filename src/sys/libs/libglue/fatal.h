#ifndef __fatal_H__
#define __fatal_H__
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
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
/** \file fatal.h
 * \brief Print the message and exit
 */

#include <macros.h>

__BEGIN_DECLS

/**
  * \brief Prints out the message and halts the CPU
  * \param message a message to be printed - printf formated
  */
void fatal(const char * message, ...) NORETURN;

__END_DECLS

#endif /* __fatal_H__ */
