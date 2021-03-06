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
#include <stdio.h>

void abort(void)
{
  printf("abort called...");
  int i;
  for (i=0; i < 2147483647; i++);
  halt();
}
