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
/** @file __generateOffsets__.cpp
 * @brief Member offset generator for use in assembler
 *
 * offsetof(klass, member) cannot be used in assembler. Therefore, we process this file, generate
 * assembly from it and grep/sed out the parts that represent offsets. That way the new file
 * "offsets.h" is generated in compile-time and it has true offsets of the class members that are
 * important for assembly.
 *
 * The idea came from Keith Owens who proposed that for Linux 2.5.x.
 */
/* NOTE: This file is not linked into the kernel. It is being just used to generate offsets.h */

#include <types.h>
#include <cstddef>
#include <macros.h>

#include <OOMTK/OProcess>
#include <OOMTK/OCPU>

#include <ia32/registers.h>

/// @brief Define the offset of a field in a structure/class for assembly
#define ASM_OFFSETOF(sym, struct, field) \
  GNU_ASM("\n#define OFFSETOF_" #sym " %0" :: "i" (offsetof(struct, field)))
  
/// @brief Define the size of a structure/clas for assembly
#define ASM_SIZEOF(type) \
  GNU_ASM("\n#define SIZEOF_" #type " %0" :: "i" (sizeof(type)))
  
/// @brief State of the stack on trap entry
struct hardwareTrapSave
{
  uint32_t  vecno;
  uint32_t  error;
  uint32_t  eip;
  uint16_t  cs;
  uint16_t  : 16;
  uint32_t  eflags;
  
  // only if not in ring0:
  uint32_t  esp;
  uint16_t  ss;
  uint16_t  : 16;
};

void __generateOffsets__(void)
{
  // NOTE: This function is a friend to all these classes.
  ASM_OFFSETOF(OProcess_FixedRegs, OProcess, m_regs.fixed);
  ASM_OFFSETOF(OProcess_Reentry_SP, OProcess, m_regs.fixed.ES);
  
  ASM_OFFSETOF(Trap_ErrorCode, struct hardwareTrapSave, error);
  ASM_OFFSETOF(Trap_EIP, struct hardwareTrapSave, eip);
  ASM_OFFSETOF(Trap_CS, struct hardwareTrapSave, cs);
  ASM_OFFSETOF(Trap_EFLAGS, struct hardwareTrapSave, eflags);
  ASM_OFFSETOF(Trap_ESP, struct hardwareTrapSave, esp);
  ASM_OFFSETOF(Trap_SS, struct hardwareTrapSave, ss);
  
  ASM_OFFSETOF(OProcess_Issues, OProcess, m_issues);
  
  ASM_OFFSETOF(OProcess_CS, OProcess, m_regs.fixed.CS);
  ASM_OFFSETOF(OProcess_DS, OProcess, m_regs.fixed.DS);
  ASM_OFFSETOF(OProcess_ES, OProcess, m_regs.fixed.ES);
  ASM_OFFSETOF(OProcess_FS, OProcess, m_regs.fixed.FS);
  ASM_OFFSETOF(OProcess_GS, OProcess, m_regs.fixed.GS);
  ASM_OFFSETOF(OProcess_SS, OProcess, m_regs.fixed.SS);
  ASM_OFFSETOF(OProcess_EAX, OProcess, m_regs.fixed.EAX);
  ASM_OFFSETOF(OProcess_EBX, OProcess, m_regs.fixed.EBX);
  ASM_OFFSETOF(OProcess_ECX, OProcess, m_regs.fixed.ECX);
  ASM_OFFSETOF(OProcess_EDX, OProcess, m_regs.fixed.EDX);
  ASM_OFFSETOF(OProcess_ESI, OProcess, m_regs.fixed.ESI);
  ASM_OFFSETOF(OProcess_EDI, OProcess, m_regs.fixed.EDI);
  ASM_OFFSETOF(OProcess_EBP, OProcess, m_regs.fixed.EBP);
  ASM_OFFSETOF(OProcess_ESP, OProcess, m_regs.fixed.ESP);
  ASM_OFFSETOF(OProcess_EIP, OProcess, m_regs.fixed.EIP);
  ASM_OFFSETOF(OProcess_EFLAGS, OProcess, m_regs.fixed.EFLAGS);
  
  ASM_OFFSETOF(OProcess_OnCPU, OProcess, m_onCPU);
  ASM_OFFSETOF(OCPU_stackTop, OCPU, m_topOfStack);
  
  ASM_OFFSETOF(Fixed_CS, fixed_regs_t, CS);
  ASM_OFFSETOF(Fixed_DS, fixed_regs_t, DS);
  ASM_OFFSETOF(Fixed_ES, fixed_regs_t, ES);
  ASM_OFFSETOF(Fixed_FS, fixed_regs_t, FS);
  ASM_OFFSETOF(Fixed_GS, fixed_regs_t, GS);
  ASM_OFFSETOF(Fixed_SS, fixed_regs_t, SS);
  ASM_OFFSETOF(Fixed_EAX, fixed_regs_t, EAX);
  ASM_OFFSETOF(Fixed_EBX, fixed_regs_t, EBX);
  ASM_OFFSETOF(Fixed_ECX, fixed_regs_t, ECX);
  ASM_OFFSETOF(Fixed_EDX, fixed_regs_t, EDX);
  ASM_OFFSETOF(Fixed_ESI, fixed_regs_t, ESI);
  ASM_OFFSETOF(Fixed_EDI, fixed_regs_t, EDI);
  ASM_OFFSETOF(Fixed_EBP, fixed_regs_t, EBP);
  ASM_OFFSETOF(Fixed_ESP, fixed_regs_t, ESP);
  ASM_OFFSETOF(Fixed_EIP, fixed_regs_t, EIP);
  ASM_OFFSETOF(Fixed_EFLAGS, fixed_regs_t, EFLAGS);
  ASM_OFFSETOF(Fixed_Error, fixed_regs_t, Error);
  ASM_OFFSETOF(Fixed_PFAddr, fixed_regs_t, ExceptAddr);
  ASM_OFFSETOF(Fixed_VecNo, fixed_regs_t, ExceptNo);
  ASM_OFFSETOF(Fixed_Reentry_SP, fixed_regs_t, ES);
  
//   ASM_OFFSETOF(TSS_Ring0_SP, ia32_TSS, esp0);
  
  ASM_SIZEOF(OProcess);
  ASM_SIZEOF(OCPU);
}
