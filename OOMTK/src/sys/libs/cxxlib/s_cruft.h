#ifndef __s_cruft_H__
#define __s_cruft_H__
/*
 *  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file s_cruft.h
 * \brief Taken from L4 (will be removed l8r)
 */
/*
 * $Id$
 */

#include <cstddef>			// for size_t

extern "C"
{
  __attribute__((noreturn)) void *smalloc(size_t);
  __attribute__((noreturn)) void sfree(void *, size_t);
};

#endif /* __s_cruft_H__ */
