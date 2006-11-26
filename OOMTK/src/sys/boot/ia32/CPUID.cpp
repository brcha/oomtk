INTERFACE:
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** @file
 * @brief CPU Identicifation
 */

#include <types.h>

/**
 * @brief CPUID class identifies the CPU
 */
class CPUID {
  public:
    /**
     * @brief CPU Vendor
     */
    enum cpuVendor {
      Unknown   = 0x0u,
      Intel     = 0x1u,
      AMD       = 0x2u,
      TransMeta = 0x3u,
    };

    /**
     * @brief CPU Features
     *
     * Names of the features are the same as in IA-32 manual (with excluded '-'
     * in some names and with 'a' added in front of AMD's 3DNow features).
     *
     * See IA-32 Intel Architecture Software Developer's Manual, Volume 2A:
     * Instruction Set Reference, A-M, pages 165-179.
     * Also see AMD CPUID Specification and AMD64 Architecture Programmer's Manual,
     * Volume 3: General-Purpose and System Instructions, pages 102-103.
     */
    enum cpuFeatures {
      // eax = 1, ecx features
      SSE3 = 0,   ///< @brief Streaming SIMD Extensions 3 (SSE3)
      MONITOR,    ///< @brief Monitor/MWait support
      DSCPL,      ///< @brief CPL Qualified Debug Store
      VMX,        ///< @brief Virtual Machine Extensions
      EST,        ///< @brief Enhanced Intel SpeedStep technology
      TM2,        ///< @brief Thermal Monitor 2
      CNXTID,     ///< @brief L1 Context ID
      CMPXCHG16B, ///< @brief CMPXCHG16B instruction supported
      // eax = 1, edx features
      FPU,        ///< @brief Floating Point Unit On-Chip
      VME,        ///< @brief Virtual 8086 Mode Enhancements
      DE,         ///< @brief Debugging Extensions - I/O breakpoints, CR4.DE, ...
      PSE,        ///< @brief Page Size Extension - large 4 MB pages
      TSC,        ///< @brief Time Stamp Counter - CR4.TSD & RDTSC instruction
      MSR,        ///< @brief Model Specific Registers - {RD,WR}MSR
      PAE,        ///< @brief Physical Address Extension - address > 32bits
      MCE,        ///< @brief Machine Check Exception - exc.18
      CX8,        ///< @brief CMPXCHG8B instruction supported
      APIC,       ///< @brief APIC On-Chip (Advanced PIC)
      SEP,        ///< @brief SYSENTER & SYSEXIT instructions
      MTRR,       ///< @brief Memory Type Range Registers
      PGE,        ///< @brief PTE Global Bit
      MCA,        ///< @brief Machine Check Architecture
      CMOV,       ///< @brief Conditional Move instruction supported
      PAT,        ///< @brief Page Attribute Table
      PSE36,      ///< @brief 36bit Page Size Extension - 4 MB pages beyond 4 GB
      PSN,        ///< @brief Processor Serial Number (96bit)
      CLFSH,      ///< @brief CLFLUSH instruction
      DS,         ///< @brief Debug Store
      ACPI,       ///< @brief Thermal Monitor and Software Controlled Clock Facilities
      MMX,        ///< @brief Intel MMX Technology
      FXSR,       ///< @brief FXSAVE and FXRSTOR instructions (for FPU context)
      SSE,        ///< @brief SSE extensions
      SSE2,       ///< @brief SSE2 extensions
      SS,         ///< @brief Self Snoop
      HTT,        ///< @brief Multi-Threading - more than one logical processor
      TM,         ///< @brief Thermal Monitor
      PBE,        ///< @brief Pending Break Enable
      // eax = 80000001, ecx features
      LahfSahf,   ///< @brief LAHF/SAHF available in 64bit mode
      CmpLegacy,  ///< @brief CMPLEGACY available (Athlon64 and Opteron)
      SVM,        ///< @brief AMD's Secure Virtual Machine feature
      AltMovCr8,  ///< @brief LOCK MOV CR0 means MOV CR8
      // eax = 80000001, edx features
      SYSCALL,    ///< @brief SYSCALL and SYSRET instructions
      FCMOV,      ///< @brief TransMeta FCMOV instruction
      NX,         ///< @brief No-eXecute page protection
      MmxExt,     ///< @brief AMD's extensions to MMX instructions
      FFXSR,      ///< @brief FXSAVE and FXRSTOR instruction optimizations
      RDTSCP,     ///< @brief RDTSCP instruction
      LM,         ///< @brief AMD's Long Mode (64-bit mode)
      EM64T,      ///< @brief Intel's EM64T mode (64-bit mode)
      a3DNowExt,  ///< @brief AMD's extensions for 3DNow! instructions
      a3DNow,     ///< @brief 3DNow! instructions
      // eax = 80000007, edx features (APM information on AMD)
      TS,         ///< @brief Temperature sensor
      FID,        ///< @brief Frequency ID control
      VID,        ///< @brief Voltage ID control
      TTP,        ///< @brief THERMTRIP is supported
      TM,         ///< @brief Hardware thermal control is supported
      STC,        ///< @brief Software thermal control is supported
      TscInvariant, ///< @brief TSC rate is ensured to be invariant across all P,C-States and stop-grant transitions (such as STPCLK Throttling) => TSC is good as a time source.
    };

  protected:
    /// @brief Some constants used in the code
    enum {
      // Registers used in features definition list
      REG_EAX   = 0,
      REG_EBX   = 1,
      REG_ECX   = 2,
      REG_EDX   = 3,
    };

    /**
     * @brief Structure that defines the features of this CPU
     */
    struct features_t {
      uint32_t  code;
      uint32_t  reg;
      const char * name;
      uint32_t  bit;
      uint32_t  vendors;
      bool      has;
    };

    /**
     * @brief Features of this CPU
     */
//     features_t features[NUM_FEATURES];

    /// @brief Registers for the cpuid() function
    struct cpuidRegisters {
      uint32_t eax;
      uint32_t ebx;
      uint32_t ecx;
      uint32_t edx;
    };
};

IMPLEMENTATION:

#include <stdio.h>
#include <VgaConsole.h>
#include <types.h>
#include INC_ARCH(EFLAGS.h)

PUBLIC void CPUID::identify()
{
  if(!isPentium())
    printf(ANSI_FG_RED "This is not Pentium!!!\n");
  else
    printf(ANSI_NORMAL "This is Pentium or greater.\n");

  if (!supportsCPUID())
    printf(ANSI_FG_RED "This CPU doesn't support CPUID instruction!!!\n");
  else
    printf(ANSI_NORMAL "This CPU supports CPUID instruction.\n");
};

PROTECTED bool CPUID::isPentium()
{
  uint32_t pentium;

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
  uint32_t cpuidSupported;

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

/**
 * @brief Calls the CPUID function
 * @param code cpuid code (-> eax)
 * @param regs returned cpuid registers
 * @returns contents of eax
 */
PROTECTED inline uint32_t CPUID::cpuid(uint32_t code, cpuidRegisters & regs)
{
  GNU_ASM(
      "   movl  %[code], %%eax    \n" /* Put the code where it belongs */
      "   cpuid                   \n" /* Call the CPUID                */
      "   movl  %%eax, %[eax]     \n" /* Save the registers            */
      "   movl  %%ebx, %[ebx]     \n"
      "   movl  %%ecx, %[ecx]     \n"
      "   movl  %%edx, %[edx]     \n"
  : [eax] "=a" (regs.eax), [ebx] "=b" (regs.ebx),
    [ecx] "=c" (regs.ecx), [edx] "=d" (regs.edx)
  : [code] "a" (code)
         );
  return regs.eax;
};

#define CPUFEATURE(code, reg, name, bit, vendor)      \
  features[name].code     = code;                     \
  features[name].reg      = REG_##reg;                \
  features[name].name     = #name;                    \
  features[name].bit      = bit;                      \
  features[name].vendors  = vendor;                   \
  features[name].has      = false;

PUBLIC CPUID::CPUID()
{
  // Initialize the CPU features

};