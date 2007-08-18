/*
 *  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file _exit.c
 * \brief Description
 */
/*
 * $Id$
 */
#include "_exit.h"

#include <halt.h>

void _exit(void)
{
  halt();
}
