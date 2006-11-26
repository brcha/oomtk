#ifndef __IA32_CPUID_H__
#define __IA32_CPUID_H__
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** @file
 * @brief IA32 CPUID Features
 */

/**
 * @defgroup ECX
 * @{
 */
/// @brief SSE3 supported
#define CPUID_ECX_SSE3            0x00000001
/// @brief Monitor/mwait (Intel only)
#define CPUID_ECX_INTEL_MONITOR   0x00000004
/// @brief DS-CPL: CPL-qualified debug store
#define CPUID_ECX_INTEL_DSCPL     0x00000010
/// @brief Virtual Machine eXtensions (Intel)
#define CPUID_ECX_INTEL_VMX       0x00000020
/// @brief Enhanced Speed Step (Intel)
#define CPUID_ECX_INTEL_EST       0x00000080
/// @brief Thermal monitor 2 (Intel)
#define CPUID_ECX_INTEL_TM2       0x00000100
/// @brief Level1 Cache Context ID (Intel)
#define CPUID_ECX_INTEL_CID       0x00000400
/// @brief 16byte cmpxchg
#define CPUID_ECX_CMPXCHG16B      0x00002000
/** @} */

/**
 * @defgroup EDX
 * @{
 */
/// @brief FPU on chip => 387+
#define CPUID_EDX_FPU             0x00000001
/// @brief Virtual Mode Extension
#define CPUID_EDX_VME             0x00000002
/// @brief Debugging Extension
#define CPUID_EDX_DE              0x00000004
/// @brief 4M Page Size Extension
#define CPUID_EDX_PSE             0x00000008
/// @brief Time Stamp Counter present
#define CPUID_EDX_TSC             0x00000010
/// @brief Model Specific Registers using {RD,WR}MSR
#define CPUID_EDX_MSR             0x00000020
/// @brief Physical Address Extension
#define CPUID_EDX_PAE             0x00000040
/// @brief Machine Check Exception
#define CPUID_EDX_MCE             0x00000080
/// @brief CMPXCHG8 instruction supported
#define CPUID_EDX_CX8             0x00000100
/// @brief On-board APIC
#define CPUID_EDX_APIC            0x00000200
/// @brief Fast system call support (sysenter/exit)
#define CPUID_EDX_SEP             0x00000800
/// @brief Memory Type Range Registers
#define CPUID_EDX_MTRR            0x00001000
/// @brief Page Global Extension
#define CPUID_EDX_PGE             0x00002000
/// @brief Machine Check Architecture
#define CPUID_EDX_MCA             0x00004000
/// @brief Conditional MOVe present
#define CPUID_EDX_CMOV            0x00008000
/// @brief Page Attribute Table
#define CPUID_EDX_PAT             0x00010000
/// @brief 36-bit Page Size Extension
#define CPUID_EDX_PSE36           0x00020000
/// @brief Processor Serial Number present and enabled (Intel only)
#define CPUID_EDX_INTEL_PSN       0x00040000
/// @brief Cache-Line FluSH instruction supported
#define CPUID_EDX_CLFSH           0x00080000
/// @brief Debug Store (Intel only)
#define CPUID_EDX_INTEL_DS        0x00200000
/// @brief Thermal monitor and SW clock ACPI features (Intel)
#define CPUID_EDX_INTEL_ACPI      0x00400000
/// @brief MMX supported
#define CPUID_EDX_MMX             0x00800000
/// @brief Fast floating point save/restore
#define CPUID_EDX_FXSR            0x01000000
/// @brief SSE supported
#define CPUID_EDX_SSE             0x02000000
/// @brief SSE2 supported
#define CPUID_EDX_SSE2            0x04000000
/// @brief Self-Snoop (Intel only)
#define CPUID_EDX_INTEL_SS        0x08000000
/// @brief Hyper-Threading supported
#define CPUID_EDX_HT              0x10000000
/// @brief Thermal monitoring (Intel)
#define CPUID_EDX_INTEL_TM        0x20000000
/// @brief Signal break on FERR (Intel)
#define CPUID_EDX_INTEL_SBF       0x80000000
/** @} */

#endif /* __IA32_CPUID_H__ */
