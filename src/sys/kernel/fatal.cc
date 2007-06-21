/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file fatal.h
 * \brief Print the message and exit
 */
/*
 * $Id$
 */

#include "fatal.h"

#include <stdio.h>
#include <halt.h>
#include <stdarg.h>

void fatal(const char * message, ...)
{
  va_list args;

  // Print the message
  va_start(args, message);
  vprintf(message, args);
  va_end(args);

  // Halt the CPU
  halt();
}
