#ifndef __HALT_H__
#define __HALT_H__
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** @file
 * @brief Halt the CPU
 */

extern "C" {
  /**
   * @brief Halt the CPU
   *
   * This function is defined in boot/<arch>/startup.S file
   */
  extern void halt() NORETURN;
};

#endif /* __HALT_H__ */
