/*
 *  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file abort.cc
 * \brief Description
 */
/*
 * $Id$
 */
#include "abort.h"

#include <halt.h>

void abort(void)
{
  halt();
}
