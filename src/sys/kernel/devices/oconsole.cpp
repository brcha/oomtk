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
/** \file
 * \brief Abstract Console class
 */
#include "oconsole.h"
#include <cstddef>

// Initialize static members
OConsole * OConsole::stdout = 0;
OConsole * OConsole::stdin  = 0;
OConsole * OConsole::stderr = 0;

/**
 * Disable stdin, stdout and stderr console.
 *
 * NOTE:
 */
void OConsole::disableAll()
{
  stdout = 0;
  stdin  = 0;
  stderr = 0;
}

/**
 * Set current console state
 * \param newState new state
 */
void OConsole::state(word_t newState)
{
  m_state = newState;
}

/**
 * Get console attributes
 * \returns console attributes
 *
 * NOTE: There is no setter command, since console attributes are not to be set by
 * users. Console attributes are properties of the console.
 */
word_t OConsole::attributes() const
{
  return INVALID;
}

/**
 * Write a string to the output.
 * \param string the string to be written (non-zero terminated)
 * \param length the length of the string
 * \returns 1 on success, 0 else.
 *
 * NOTE: This method should be implemented in the implementations of the Console.
 * NOTE: This method is used to connect the console with the libc glue.
 */
int OConsole::write(char const * string, size_t length)
{
  return 0;
}

/**
 * Get a character from the input stream.
 * \param blocking true if this method should block until character is available.
 * \returns -1 if not implemented, or character unavailable.
 *
 * NOTE: This method should be implemented in the implementations of the Console.
 * NOTE: This method is used to connect the console with the libc glue.
 */
int OConsole::getchar(bool blocking)
{
  return -1;
}

/**
 * Is the character available for reading?
 * \returns -1 if not implemented, 0 if no character is available and 1 if at least one character is available
 *
 * NOTE: This metod should be implemented in the implementations of the Console.
 */
int OConsole::charAvailable() const
{
  return -1;
}
