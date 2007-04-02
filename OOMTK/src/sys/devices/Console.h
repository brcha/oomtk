#ifndef __Console_H__
#define __Console_H__
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file Console.h
 * \brief Abstract Console class
 */
/*
 * $Id$
 */

#include <cstddef>
#include <types.h>

class Console
{
  public:
    /**
     * Disable stdin, stdout and stderr console.
     */
    static void disableAll();

    /**
     * Get current console state
     * \returns current console state
     */
    inline word_t state() const
    {
      return m_state;
    }

    /**
     * Set current console state
     * \param newState new state
     */
    virtual void state(word_t newState);

    /**
     * Get console attributes
     * \returns console attributes
     *
     * NOTE: There is no setter command, since console attributes are not to be set by
     * users. Console attributes are properties of the console.
     */
    virtual word_t attributes() const;

    /**
     * Write a string to the output.
     * \param string the string to be written (non-zero terminated)
     * \param length the length of the string
     * \returns 1 on success, 0 else.
     *
     * NOTE: This method should be implemented in the implementations of the Console.
     * NOTE: This method is used to connect the console with the libc glue.
     */
    virtual int write(char const * string, size_t length);

    /**
     * Get a character from the input stream.
     * \param blocking true if this method should block until character is available.
     * \returns -1 if not implemented, or character unavailable.
     *
     * NOTE: This method should be implemented in the implementations of the Console.
     * NOTE: This method is used to connect the console with the libc glue.
     */
    virtual int getchar(bool blocking = true);

    /**
     * Is the character available for reading?
     * \returns -1 if not implemented, 0 if no character is available and 1 if at least one character is available
     *
     * NOTE: This metod should be implemented in the implementations of the Console.
     */
    virtual int charAvailable() const;

  protected:
    /**
     * \brief Don't construct this class!
     */
    Console() {};

  public:
    enum State
    {
      DISABLED  = 0,  ///< Console disabled
      INPUT     = 1,  ///< Console input enabled
      OUTPUT    = 2,  ///< Console output enabled
    };

    enum Attributes
    {
      // Universal attributes
      INVALID   = 0,
      IN        = 1 << 0, ///< input from the console is possible
      OUT       = 1 << 1, ///< output to the console is possible
      // Attributes that describe the console
      DIRECT    = 1 << 2, ///< Keyboard/screen input/output
      UART      = 1 << 3, ///< Serial line I/O
      DEBUG     = 1 << 4, ///< Debugging output
      BUFFERED  = 1 << 5, ///< Buffered console
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

#endif /* __Console_H__ */
