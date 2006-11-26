#ifndef __ASM_MACROS_H__
#define __ASM_MACROS_H__
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/*
 * $Id: asmMacros.h 196 2006-10-23 13:29:52Z brcha $
 */
/** @file
 * @brief Assembly macros
 */

/**
 * @brief Entry point for a procedure that is called from C
 */
#define ASENTRY(proc) .align 2; .globl proc; .type proc,@function; proc:
#define ENTRY(proc) ASENTRY(## proc)

/**
 * @brief Align a branch target and fill the gap with nop's
 */
#define ALIGN_TEXT .align 2,0x90
#define SUPERALIGN_TEXT .p2align 4,0x90 /* 16-byte alignment */

/**
 * @brief Gas won't do logical shifts in computed immediates :(
 */
#define SRL(val, howMuch) (((val) >> (howMuch)) & ~(-1 << (32 - (howMuch))))

#define DEF_SYM(symbol, address) \
  asm(".globl _" #symbol "\n"    \
      "\t.set _" #symbol ",%P0"  \
      :: "i" (address))

#endif /* __ASM_MACROS_H__ */
