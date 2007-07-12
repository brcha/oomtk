/*
 *  Copyright (C) 2007 by Filip Brčić <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK (http://launchpad.net/oomtk)
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
/** \file __generateOffsets.cpp
 * \brief Member offset generator for use in assembler
 *
 * offsetof(klass, member) cannot be used in assembler. Therefore, we process this file, generate
 * assembly from it and grep/sed out the parts that represent offsets. That way the new file
 * "offsets.h" is generated in compile-time and it has true offsets of the class members that are
 * important for assembly.
 *
 * The idea came from Keith Owens who proposed that for Linux 2.5.x.
 */
/* NOTE: This file is not linked into the kernel. It is being just used to generate offsets.h */

#include <Context.h>
#include <Process.h>

#include <stddef.h>

/// Define the SYMbol in assembly so that it could be extracted later using grep & sed
#define OFF_DEF(sym, struct, field) \
  GNU_ASM("\n#define " #sym " %0" :: "i" (offsetof(struct, field)))

void __generateOffsets__(void)
{
  // NOTE: This function is a friend to all these classes.
  OFF_DEF(OFFSET_PROCESS_CTX, Process, ctx);
  OFF_DEF(OFFSET_CONTEXT_CS, Context, CS);
  OFF_DEF(OFFSET_CONTEXT_DS, Context, DS);
  OFF_DEF(OFFSET_CONTEXT_ES, Context, ES);
  OFF_DEF(OFFSET_CONTEXT_FS, Context, FS);
  OFF_DEF(OFFSET_CONTEXT_GS, Context, GS);
  OFF_DEF(OFFSET_CONTEXT_SS, Context, SS);
}
