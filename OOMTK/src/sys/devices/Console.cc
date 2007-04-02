/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file
 * \brief Abstract Console class
 */
/*
 * $Id$
 */

#include "Console.h"
#include <cstddef>

// Initialize static members
Console * Console::stdout = 0;
Console * Console::stdin  = 0;
Console * Console::stderr = 0;

/**
 * Disable stdin, stdout and stderr console.
 *
 * NOTE:
 */
static void Console::disableAll()
{
  stdout = 0;
  stdin  = 0;
  stderr = 0;
}

/**
 * Set current console state
 * \param newState new state
 */
virtual void Console::state(word_t newState)
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
virtual word_t Console::attributes() const
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
virtual int Console::write(char const * string, size_t length)
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
virtual int Console::getchar(bool blocking = true)
{
  return -1;
}

/**
 * Is the character available for reading?
 * \returns -1 if not implemented, 0 if no character is available and 1 if at least one character is available
 *
 * NOTE: This metod should be implemented in the implementations of the Console.
 */
virtual int Console::charAvailable() const
{
  return -1;
}
