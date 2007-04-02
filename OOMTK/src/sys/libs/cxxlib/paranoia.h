#ifndef __paranoia_H__
#define __paranoia_H__
/*
 *  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file paranoia.h
 * \brief Paranoid functions (taken from L4)
 */
/*
 * $Id$
 */

#include <cstddef>

extern "C"
{
  void __cxa_pure_virtual();
  void __pure_virtual();
  void __div0(void);
}

void operator delete(void *);

#endif /* __paranoia_H__ */
