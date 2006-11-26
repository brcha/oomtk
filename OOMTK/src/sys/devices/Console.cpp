INTERFACE:
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/*
 * $Id: Console.cpp 196 2006-10-23 13:29:52Z brcha $
 */
/** @file
 * @brief Abstract Console class
 */

#include <cstddef>
#include <types.h>

class Console
{
  public:
    enum State {
      DISABLED  = 0,  ///< Console disabled
      INPUT     = 1,  ///< Console input enabled
      OUTPUT    = 2,  ///< Console output enabled
    };

    enum Attributes {
      // Universal attributes
      INVALID   = 0,
      IN        = 1<<0,   ///< input from the console is possible
      OUT       = 1<<1,   ///< output to the console is possible
      // Attributes that describe the console
      DIRECT    = 1<<2,   ///< Keyboard/screen input/output
      UART      = 1<<3,   ///< Serial line I/O
      DEBUG     = 1<<4,   ///< Debugging output
      BUFFERED  = 1<<5,   ///< Buffered console
    };

  public:

    /// Standard output
    static Console * stdout;
    /// Standard input
    static Console * stdin;
    /// Standard error
    static Console * stderr;

  protected:
    word_t  m_state;
};

IMPLEMENTATION:

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
PUBLIC static void Console::disableAll()
{
  stdout = 0;
  stdin  = 0;
  stderr = 0;
}

/**
 * Get current console state
 * @returns current console state
 */
PUBLIC inline word_t Console::state() const
{
  return m_state;
}

/**
 * Set current console state
 * @param newState new state
 */
PUBLIC virtual void Console::state(word_t newState)
{
  m_state = newState;
}

/**
 * Get console attributes
 * @returns console attributes
 *
 * NOTE: There is no setter command, since console attributes are not to be set by
 * users. Console attributes are properties of the console.
 */
PUBLIC virtual word_t Console::attributes() const
{
  return INVALID;
}

/**
 * Write a string to the output.
 * @param string the string to be written (non-zero terminated)
 * @param length the length of the string
 * @returns 1 on success, 0 else.
 *
 * NOTE: This method should be implemented in the implementations of the Console.
 * NOTE: This method is used to connect the console with the libc glue.
 */
PUBLIC virtual int Console::write(char const * string, size_t length)
{
  return 0;
}

/**
 * Get a character from the input stream.
 * @param blocking true if this method should block until character is available.
 * @returns -1 if not implemented, or character unavailable.
 *
 * NOTE: This method should be implemented in the implementations of the Console.
 * NOTE: This method is used to connect the console with the libc glue.
 */
PUBLIC virtual int Console::getchar(bool blocking = true)
{
  return -1;
}

/**
 * Is the character available for reading?
 * @returns -1 if not implemented, 0 if no character is available and 1 if at least one character is available
 *
 * NOTE: This metod should be implemented in the implementations of the Console.
 */
PUBLIC virtual int Console::charAvailable() const
{
  return -1;
}

/**
 * @brief Don't construct this class!
 */
PROTECTED Console::Console()
{
};