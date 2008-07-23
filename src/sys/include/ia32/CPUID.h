#ifndef __IA32_CPUID_H__
#define __IA32_CPUID_H__
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
/** @file CPUID.h
 * @brief CPUID feature flags (function 1)
 */

// ECX features
#define CPUID_ECX_SSE3            0x00000001
#define CPUID_ECX_CMPXCHG16B      0x00002000
#define CPUID_ECX_INTEL_MONITOR   0x00000004
#define CPUID_ECX_INTEL_DSCPL     0x00000010
#define CPUID_ECX_INTEL_VMX       0x00000020
#define CPUID_ECX_INTEL_EST       0x00000080
#define CPUID_ECX_INTEL_TM2       0x00000100
#define CPUID_ECX_INTEL_CID       0x00000400

// EDX features
#define CPUID_EDX_FPU             0x00000001
#define CPUID_EDX_VME             0x00000002
#define CPUID_EDX_DE              0x00000004
#define CPUID_EDX_PSE             0x00000008
#define CPUID_EDX_TSC             0x00000010
#define CPUID_EDX_MSR             0x00000020
#define CPUID_EDX_PAE             0x00000040
#define CPUID_EDX_MCE             0x00000080
#define CPUID_EDX_CX8             0x00000100
#define CPUID_EDX_APIC            0x00000200
#define CPUID_EDX_SEP             0x00000800
#define CPUID_EDX_MTRR            0x00001000
#define CPUID_EDX_PGE             0x00002000
#define CPUID_EDX_MCA             0x00004000
#define CPUID_EDX_CMOV            0x00008000
#define CPUID_EDX_PAT             0x00010000
#define CPUID_EDX_PSE36           0x00020000
#define CPUID_EDX_INTEL_PSN       0x00040000
#define CPUID_EDX_CFLSH           0x00080000
#define CPUID_EDX_INTEL_DS        0x00200000
#define CPUID_EDX_INTEL_ACPI      0x00400000
#define CPUID_EDX_MMX             0x00800000
#define CPUID_EDX_FXSR            0x01000000
#define CPUID_EDX_SSE             0x02000000
#define CPUID_EDX_SSE2            0x04000000
#define CPUID_EDX_INTEL_SS        0x08000000
#define CPUID_EDX_HT              0x10000000
#define CPUID_EDX_INTEL_TM        0x20000000
#define CPUID_EDX_INTEL_SBF       0x80000000

#endif /* __IA32_CPUID_H__ */
