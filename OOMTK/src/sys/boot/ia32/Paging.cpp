/*
 *  Copyright (C) 2006-2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file
 * \brief Paging support
 *
 * \todo Is this file IA32 specific or generic???
 */
/*
 * $Id$
 */

#include "Paging.h"

#include <fatal.h>
#include <ansi.h>

// Low-level get/set operators

/**
 * \brief Get the CR0 register
 * \returns value of the CR0 register
 */
uint32_t Paging::cr0()
{
  uint32_t value;

  GNU_ASM(
      "   mov   %%cr0, %%edx     \n" // read the register
      "   movl  %%edx, %[value]  \n"
  : [value] "=m" (value)
  : /* no input */
  : "edx", "memory"
         );

  return value;
};

/**
 * \brief Set the CR0 register
 * \param value new value for the CR0 register
 *
 * \warning This method enables paging (if PG bit is set), so take care how you use it
 */
void Paging::cr0(uint32_t value)
{
  GNU_ASM(
      "   movl  %[value], %%edx   \n"
      "   mov   %%edx, %%cr0      \n" // write the register
  : /* no output */
  : [value] "m" (value)
  : "edx"
         );
};

/**
 * \brief Get the CR3 register
 * \returns value of the CR3 register
 */
uint32_t Paging::cr3()
{
  uint32_t value;

  GNU_ASM(
      "   mov   %%cr3, %%edx     \n" // read the register
      "   movl  %%edx, %[value]  \n"
  : [value] "=m" (value)
  : /* no input */
  : "edx", "memory"
         );

  return value;
};

/**
 * \brief Set the CR3 register
 * \param value new value for CR3 register
 */
void Paging::cr3(uint32_t value)
{
  GNU_ASM(
      "   movl  %[value], %%edx   \n"
      "   mov   %%edx, %%cr3      \n" // write the register
  : /* no output */
  : [value] "m" (value)
  : "edx"
         );
};

/**
 * \brief Get the CR4 register
 * \returns the value from CR4
 */
uint32_t Paging::cr4()
{
  uint32_t value;

  GNU_ASM(
      "   mov   %%cr4, %%edx     \n" // read the register
      "   movl  %%edx, %[value]  \n"
  : [value] "=m" (value)
  : /* no input */
  : "edx", "memory"
         );

  return value;
};

/**
 * \brief Set the CR4 register
 * \param value new value for CR4 register
 */
void Paging::cr4(uint32_t value)
{
  GNU_ASM(
      "   movl  %[value], %%edx   \n"
      "   mov   %%edx, %%cr4      \n" // write the register
  : /* no output */
  : [value] "m" (value)
  : "edx"
         );
};

