#ifndef __free_H__
#define __free_H__
/*
 *  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file free.h
 * \brief Free the pointer
 */
/*
 * $Id$
 */

#ifdef __cplusplus
extern "C"
{
#endif
  /**
   * \brief Free the pointer
   * @param ptr the address of the pointer to be freed
   */
  void free(void *ptr);
#ifdef __cplusplus
}
#endif

#endif /* __free_H__ */
