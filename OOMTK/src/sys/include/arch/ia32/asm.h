#ifndef __IA32_ASM_H__
#define __IA32_ASM_H__
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

// default alignment
#define ALIGN 16

#define EXT(x)  x
#define LEXT(x) x## :
#define GEXT(x) .globl EXT(x); LEXT(x)

#define ALIGNEDVAR(x,al)  .globl EXT(x); .align al; LEXT(x)
#define VAR(x)            ALIGNEDVAR(x,4)
#define ENTRY(x)          .globl EXT(x); .type EXT(x),@function; LEXT(x)
#define GDATA(x)          .globl EXT(x); .align ALIGN; LEXT(x)

#endif /* __IA32_ASM_H__ */
