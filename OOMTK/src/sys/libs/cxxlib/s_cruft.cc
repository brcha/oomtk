/*
 *  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file s_cruft.cc
 * \brief Taken from L4 (will be removed l8r)
 */
/*
 * $Id: $
 */
#include "s_cruft.h"

// #include "panic.h"			// for panic

// do not allow calls to these OSKit routines.  We have to provide
// these functions because libamm references them.

extern "C" __attribute__((noreturn)) void *smalloc(size_t)
{
//   panic("smalloc called");
}

extern "C" __attribute__((noreturn)) void sfree(void *, size_t)
{
//   panic("sfree called");
}
