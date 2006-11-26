INTERFACE:
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** @file
 * @brief CPU Identicifation
 */

/**
 * @brief CPUID class identifies the CPU
 */
class CPUID {
  public:
    /**
     * @brief CPU Class
     */
    enum cpu_t {

    };
};

IMPLEMENTATION:

#include <stdio.h>
#include <types.h>
#include INC_ARCH(EFLAGS.h)

PUBLIC void CPUID::identify()
{
  if(!isPentium())
    printf("This is not Pentium!!!\n");
  else
    printf("This is Pentium or greater.\n");

  if (!supportsCPUID())
    printf("This CPU doesn't support CPUID instruction!!!\n");
  else
    printf("This CPU supports CPUID instruction.\n");
};

PROTECTED bool CPUID::isPentium()
{
  u32_t pentium;

  GNU_ASM(
      "   pushf                 \n" /* Push flags onto the stack    */
      "   xorl  %[ac], (%%esp)  \n" /* Set the AC flag              */
      "   movl  (%%esp), %%eax  \n" /* Save the flags with AC bit   */
      "   popf                  \n" /* Pop the flags                */
      "   pushf                 \n" /* Re-push the flags            */
      "   cmpl  (%%esp), %%eax  \n" /* Check the AC bit for change  */
      "   je    1f              \n" /* If equal, then it is ok      */
      "   movl  $0, %[pentium]  \n" /* Set pentium to false         */
      "   jmp   2f              \n"
      "1: movl  $1, %[pentium]  \n" /* Set pentium to true          */
      "2: pushl %%eax           \n" /* Restore original flags       */
      "   popf                  \n"
      "   popl  %%eax           \n" /* I need the same number of push-pops */
  : [pentium] "=m" (pentium)
  : [ac] "i" (EFLAGS_AC)
         );

  return (pentium)?true:false;
};

PROTECTED bool CPUID::supportsCPUID()
{
  u32_t cpuidSupported;

  GNU_ASM(
      "   pushf                 \n" /* Push the flags onto the stack  */
      "   xorl  %[id], (%%esp)  \n" /* Set the ID bit                 */
      "   movl  (%%esp), %%eax  \n" /* Save the flags with ID bit set */
      "   popf                  \n"
      "   pushf                 \n"
      "   cmpl  (%%esp), %%eax  \n"
      "   je    1f              \n"
      "   movl  $0, %[cpuidok]  \n"
      "   jmp   2f              \n"
      "1: movl  $1, %[cpuidok]  \n"
      "2: pushl %%eax           \n"
      "   popf                  \n"
      "   popl  %%eax           \n" /* I need the same number of push-pops */
  : [cpuidok] "=m" (cpuidSupported)
  : [id] "i" (EFLAGS_ID)
  : "memory", "eax"
         );

  return (cpuidSupported)?true:false;
};

