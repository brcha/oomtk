/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** @file
 * @brief C++ ABI for gcc-4.1.1 - guard acquire & release
 *
 * @todo Add regular lock support to this file...
 */

#include <types.h>

typedef word_t guard_t;

extern "C"
{

int __cxa_guard_acquire(guard_t * g)
{
#warning This function is not finished!!!
  if (*(char*)(g) != 0) // guard test
    return 0;
  ((char*)g)[1] ++;   // guard push
  return 1;
};

void __cxa_guard_abort(guard_t * g)
{
#warning This function is not finished!!!
  --((char*)g)[1];    // guard pop
};

void __cxa_guard_release(guard_t * g)
{
#warning This function is not finished!!!
  --((char*)g)[1];    // guard pop
  *(char*)(g) = 1;  // guard set
};

void _Unwind_Resume()
{
}

};
