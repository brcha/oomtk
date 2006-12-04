INTERFACE:
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** @file
 * @brief Paging support
 *
 * @todo Is this file IA32 specific or generic???
 */

#include <types.h>

/**
 * @brief Paging base class
 *
 * This is the base class for paging support. It has to be inherited and implemented
 * for specific architecture and specific paging method.
 */
class Paging
{
};

/**
 * @brief Enable debugging messages or not
 */
#define DEBUG_PAGING DEBUG_ENABLE

IMPLEMENTATION:

#include <fatal.h>
#include <ansi.h>

/**
 * @brief Inhibit creating instances of this class
 */
PROTECTED Paging::Paging()
{
};

/**
 * @brief Setup paging
 *
 * After booting, the kernel is located at physical memory location 0x00100000,
 * but kernel is relocated at 0xC0100000 (= KVA + 0x00100000 = 3 GB + 1 MB).
 *
 * This method should map the physical memory [0, 2M] to virtual [0, 2M] and
 * also to [KVA, KVA+2M].
 *
 * In case of legacy mapping, it will map [0, 4M] to virtual [0, 4M] and KVA + [0, 4M].
 */
PUBLIC virtual void Paging::setup()
{
  fatal(ANSI_FG_RED "Paging::setup is virtual function!!!\n" ANSI_NORMAL);
};

// Low-level get/set operators

/**
 * @brief Get the CR0 register
 * @returns value of the CR0 register
 */
PROTECTED uint32_t Paging::cr0()
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
 * @brief Set the CR0 register
 * @param value new value for the CR0 register
 *
 * @warning This method enables paging (if PG bit is set), so take care how you use it
 */
PROTECTED void Paging::cr0(uint32_t value)
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
 * @brief Get the CR3 register
 * @returns value of the CR3 register
 */
PROTECTED uint32_t Paging::cr3()
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
 * @brief Set the CR3 register
 * @param value new value for CR3 register
 */
PROTECTED void Paging::cr3(uint32_t value)
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
 * @brief Get the CR4 register
 * @returns the value from CR4
 */
PROTECTED uint32_t Paging::cr4()
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
 * @brief Set the CR4 register
 * @param value new value for CR4 register
 */
PROTECTED void Paging::cr4(uint32_t value)
{
  GNU_ASM(
      "   movl  %[value], %%edx   \n"
      "   mov   %%edx, %%cr4      \n" // write the register
  : /* no output */
  : [value] "m" (value)
  : "edx"
         );
};

