#ifndef __abort_H__
#define __abort_H__
/*
 *  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file _exit.h
 * \brief _exit - cause abnormal program termination
 */
/*
 * $Id$
 */

#ifdef __cplusplus
extern "C"
{
#endif
  /**
   * \brief Cause abnormal program termination
   */
  void _exit(void);
#ifdef __cplusplus
}
#endif

#endif /* __abort_H__ */
