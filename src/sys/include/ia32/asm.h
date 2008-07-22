#ifndef __IA32_ASM_H__
#define __IA32_ASM_H__
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@gna.org>
 *
 *  This file is part of OOMTK
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
