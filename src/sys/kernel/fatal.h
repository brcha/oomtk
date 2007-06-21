#ifndef __fatal_H__
#define __fatal_H__
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

extern "C" {
  /**
   * \brief Prints out the message and halts the CPU
   * \param message a message to be printed - printf formated
   */
  void fatal(const char * message, ...) NORETURN;
};

#endif /* __fatal_H__ */
