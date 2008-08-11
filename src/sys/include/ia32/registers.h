#ifndef __IA32_REGISTERS_H__
#define __IA32_REGISTERS_H__
/*
 *  Copyright (C) 2008 by Filip Brcic <brcha@gna.org>
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
/** @file registers.h
 * @brief IA32 register structures
 */

#include <types.h>
#include <cstddef>

/**
 * @brief Fixed-point registers layout
 */
typedef struct ia32_fixed_regs_t
{
  uint32_t EDI;
  uint32_t ESI;
  uint32_t EBP;
  uint32_t ExceptAddr;
  uint32_t EBX;
  uint32_t EDX;
  uint32_t ECX;
  uint32_t EAX;
  uint32_t ExceptNo;
  uint32_t Error;
  uint32_t EIP;
  uint32_t CS;
  uint32_t EFLAGS;
  uint32_t ESP;
  uint32_t SS;
  uint32_t ES;
  uint32_t DS;
  uint32_t FS;
  uint32_t GS;
} ia32_fixed_regs_t;

typedef uint32_t ia32_ssereg_t[4];

/**
 * @brief Floating point and SIMD registers layout
 * 
 * @note Layout is for FXSAVE, not the obsolete FSAVE
 */
typedef struct ia32_fxsave_t
{
  uint16_t      fcw;
  uint16_t      fsw;
  uint16_t      ftw;
  uint16_t      fop;
  uint32_t      eip;
  uint32_t      cs;
  uint32_t      dp;
  uint32_t      ds;
  uint32_t      mxcsr;
  uint32_t      mxcsr_mask;
  ia32_ssereg_t fpr[8];     // MMX
  ia32_ssereg_t xmm[8];
} ia32_fxsave_t;

typedef union ia32_float_regs_t
{
  ia32_fxsave_t fxregs;
  uint32_t      fxsave[128] __attribute__((aligned(16)));
} ia32_float_regs_t;

typedef struct ia32_regset_t
{
  ia32_fixed_regs_t fixed;
  ia32_float_regs_t fp;
} ia32_regset_t;

#ifdef OOMTK_ARCH_IA32
typedef ia32_regset_t     regset_t;
typedef ia32_fixed_regs_t fixed_regs_t;
typedef uint32_t          flags_t;
#endif /* OOMTK_ARCH_IA32 */

#endif /* __IA32_REGISTERS_H__ */
