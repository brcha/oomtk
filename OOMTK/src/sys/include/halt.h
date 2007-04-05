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
/*
 * $Id: $
 */

#ifdef __cplusplus
extern "C"
{
#endif
  /**
   * @brief Halt the CPU
   *
   * This function is defined in boot/<arch>/startup.S file
   */
  extern void halt() NORETURN;
#ifdef __cplusplus
}
#endif

#endif /* __HALT_H__ */
