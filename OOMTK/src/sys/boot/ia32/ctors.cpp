/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file
 * \brief Static constructors and destructors
 */
/*
 * $Id: $
 */

#include "ctors.h"

// Function pointer helper type
typedef void (*func_ptr) (void);

// Zero-terminated constructor/destructor tables
extern func_ptr __ctors__[];
extern func_ptr __dtors__[];

/**
 * \brief Calls all constructors
 */
void callCtors()
{
  for(unsigned int i = 0; __ctors__[i] != 0; i++)
    __ctors__[i]();
}

/**
 * \brief Calls all destructors
 */
void callDtors()
{
  for(unsigned int i = 0; __dtors__[i] != 0; i++)
    __dtors__[i]();
}

void *__dso_handle;

struct object
{
  void (*f)(void*);
  void *p;
  void *d;
} object[32] = {0};
unsigned int iObject = 0;

int __cxa_atexit(void (*f)(void *), void *p, void *d)
{
  if (iObject >= 32)
    return -1;
  object[iObject].f = f;
  object[iObject].p = p;
  object[iObject].d = d;
  iObject ++;
  return 0;
}

void __cxa_finalize(void *d)
{
  unsigned int i = iObject;
  for (; i > 0; i --)
  {
    iObject --;
    object[iObject].f(object[iObject].p);
  }
}
