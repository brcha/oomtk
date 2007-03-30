#ifndef __ctors_H__
#define __ctors_H__
/*
 *  Copyright (C) 2006-2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file ctors.h
 * \brief Static constructors and destructors
 */
/*
 * $Id$
 */

extern "C"
{
  int __cxa_atexit(void(*f)(void *), void *p, void *d);
  void __cxa_finalize(void *d);
};

#endif /* __ctors_H__ */
