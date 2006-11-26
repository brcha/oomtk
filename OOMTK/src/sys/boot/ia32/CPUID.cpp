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
     * in some names).
     *
     * See IA-32 Intel Architecture Software Developer's Manual, Volume 2A:
     * Instruction Set Reference, A-M, pages 165-...
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
      PSE36,
      PSN,
      CLFSH,
      DS,
      ACPI,
      MMX,
      FXSR,
      SSE,
      SSE2,
      SS,
      HT,
      TM,
      SBF,
      // eax = 80000001, ecx features
      LAHF64,
      CMPLEGACY,
      SVM,
      AltMovCr8,
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