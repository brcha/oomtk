INTERFACE:
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** @file
 * @brief Print the message and exit
 */

extern "C" {
  /**
   * @brief Prints out the message and halts the CPU
   * @param message a message to be printed - printf formated
   */
  void fatal(const char * message, ...) NORETURN;
};

IMPLEMENTATION:

#include <stdio.h>
#include <halt.h>
#include <stdarg.h>

IMPLEMENT void fatal(const char * message, ...)
{
  va_list args;

  // Print the message
  va_start(args, message);
  vprintf(message, args);
  va_end(args);

  // Halt the CPU
  halt();
}
