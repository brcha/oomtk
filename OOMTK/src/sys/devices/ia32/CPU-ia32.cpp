IMPLEMENTATION [ia32]:
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/*
 * $Id: CPU-ia32.cpp 196 2006-10-23 13:29:52Z brcha $
 */
/** @file
 * @brief IA32 CPU implementation
 *
 * IA32 = i386, i486, i586, i686, pentium3, pentium4, amd64 (in 32bit mode), ...
 */

#include INC_ARCH(mmu.h)

IMPLEMENT static inline void CPU::cli()
{
  asm volatile ("cli" : : : "memory");
}

IMPLEMENT static inline void CPU::sti()
{
  asm volatile ("sti" : : : "memory");
}

IMPLEMENT static inline word_t CPU::interrupts()
{
  word_t state;
  asm volatile (
      "pushfl       \n\t"
      "popl %0      \n\t"
  : "=g"(state) : /* no input */ : "memory");
  return state & FL_IF; // return the IF flags
}

IMPLEMENT static inline word_t CPU::cliAndSave()
{
  word_t state;
  asm volatile (
      "pushfl       \n\t"
      "popl %0      \n\t"
      "cli          \n\t"
  : "=g"(state) : /* no input */ : "memory");
  return state; // Return all the flags
}

IMPLEMENT static inline void CPU::stiRestore(word_t state)
{
  if (state & FL_IF)
    asm volatile ("sti" : : : "memory");
}

IMPLEMENT static void CPU::pause()
{
  asm volatile (" .byte 0xf3, 0x90 # pause \n");
}

IMPLEMENT static void CPU::halt()
{
  asm volatile ("hlt" : : : "memory");
}

IMPLEMENT static inline void CPU::makeIrqChance()
{
  asm volatile ("nop; nop;" : : : "memory");
}

IMPLEMENT static inline void CPU::stackPointer(word_t sp)
{
  asm volatile ("movl %0, %%esp\n\t" : : "r"(sp));
}

IMPLEMENT static inline void CPU::programCounter()
{
  word_t pc;
  // Jump and Save PC
  asm volatile (
      "call 1f  \n\t"
      "1: pop %0"
  : "=rm"(pc));
  return pc;
}

