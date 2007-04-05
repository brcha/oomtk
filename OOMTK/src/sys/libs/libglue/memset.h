#ifndef __memset_H__
#define __memset_H__
/*
 *  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file memset.h
 * \brief Fill memory with a constant byte
 */
/*
 * $Id$
 */

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif
  /**
   * \brief Fill memory with a constant byte
   *
   * The memset() function fills the first n bytes of the memory area pointed to by s with the constant byte c.
   * 
   * \param s start of the memory area
   * \param c constant byte value
   * \param n number of bytes
   * \returns a pointer to the memory area s
   */
  void *memset(void *s, int c, size_t n);
#ifdef __cplusplus
}
#endif

#endif /* __memset_H__ */
