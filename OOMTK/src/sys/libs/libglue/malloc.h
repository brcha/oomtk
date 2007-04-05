#ifndef __malloc_H__
#define __malloc_H__
/*
 *  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file malloc.h
 * \brief Allocate memory
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
   * \brief Allocate the pointer in memory
   * \param size of the pointer
   * \returns the allocated pointer
   */
  void *malloc(size_t size);
#ifdef __cplusplus
}
#endif

#endif /* __malloc_H__ */
