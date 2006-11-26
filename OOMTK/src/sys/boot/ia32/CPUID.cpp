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
     *
     * These values should form a bitfield, so they should be 2^n
     */
    enum cpuVendor_t {
      Unknown   = 0x0u,
      Intel     = 0x1u,
      AMD       = 0x2u,
    };

    /**
     * @brief CPU Features
     *
     * Names of the features are the same as in IA-32 manual (with excluded '-'
     * in some names and with 'a' added in front of AMD's 3DNow features).
     *
     * For Intel see AP-485, Intel Processor Identicifation and the CPUID Instruction
     * and Intel64 and IA-32 Intel Architecture Software Developer's
     * Manual, Volume 2A: Instruction Set Reference, A-M, pages 160-188.
     *
     * For AMD see AMD's CPUID Specification and AMD64 Architecture Programmer's
     * Manual, Volume 3: General-Purpose and System Instructions, pages 102-103.
     */
    enum cpuFeatures {
      // eax = 1, ecx features
      SSE3 = 0,   ///< @brief Streaming SIMD Extensions 3 (SSE3)
      MONITOR,    ///< @brief Monitor/MWait support
      DSCPL,      ///< @brief CPL Qualified Debug Store
      VMX,        ///< @brief Virtual Machine Extensions
      EST,        ///< @brief Enhanced Intel SpeedStep technology
      TM2,        ///< @brief Thermal Monitor 2
      SSSE3,      ///< @brief Supplemental Streaming SIMD Extensions 3
      CID,        ///< @brief L1 Context ID
      CX16,       ///< @brief CMPXCHG16B instruction supported
      xTPR,       ///< @brief Send Task Priority Messages
      DCA,        ///< @brief Direct Cache Access
      RAZ,        ///< @brief AMD64 64-bit register parts access
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
      iTM,        ///< @brief Thermal Monitor (Intel)
      PBE,        ///< @brief Pending Break Enable
      // eax = 80000001, ecx features
      LahfSahf,   ///< @brief LAHF/SAHF available in 64bit mode
      CmpLegacy,  ///< @brief CMPLEGACY available (Athlon64 and Opteron)
      SVM,        ///< @brief AMD's Secure Virtual Machine feature
      AltMovCr8,  ///< @brief LOCK MOV CR0 means MOV CR8
      // eax = 80000001, edx features
      SYSCALL,    ///< @brief SYSCALL and SYSRET instructions
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
      aTM,        ///< @brief Hardware thermal control is supported (AMD)
      STC,        ///< @brief Software thermal control is supported
      TscInvariant, ///< @brief TSC rate is ensured to be invariant across all P,C-States and stop-grant transitions (such as STPCLK Throttling) => TSC is good as a time source.
      NUM_FEATURES  ///< @brief This is for size of features list
    };

  protected:
    /// @brief Some constants used in the code
    enum {
      // Registers used in features definition list
      EAX   = 0,
      EBX   = 1,
      ECX   = 2,
      EDX   = 3,
    };

    /**
     * @brief Structure that defines the features of this CPU
     */
    struct features_t {
      uint32_t  code;
      uint32_t  reg;
      char name[20]; // maximum name of feature is 19 characters ( + '\0')
      uint32_t  bit;
      uint32_t  vendor;
      bool      has;
    };

    /**
     * @brief Features of this CPU
     */
    features_t features[NUM_FEATURES];

    /// @brief Registers for the cpuid() function
    struct cpuidRegisters {
      uint32_t eax;
      uint32_t ebx;
      uint32_t ecx;
      uint32_t edx;
    };

    /// @brief CPU Vendor String ("GenuineIntel" or "AuthenticAMD")
    char _cpuVendor[13];
    /// @brief CPU Name String (functions 0x8000000[4:2], eax, ebx, ecx & edx)
    char _cpuName[49];
    /// @brief CPU Vendor ID (from cpuVendor_t enum)
    cpuVendor_t _cpuVendorID;

    // eax = 1, eax fields
    uint32_t _cpuFamily;
    uint32_t _cpuModel;
    uint32_t _cpuStepping;
    // eax = 1, ebx fields
    uint32_t _nLogicalProcessors;

    /// @brief CPU Number starting at 0 (for multiple-processor platforms support)
    uint32_t _cpuNumber;
    /// @brief Print to console or not?
    bool _printOut;

    /// @brief Maximum function
    uint32_t maximumFunction;
    /// @brief Maximum extended function
    uint32_t maximumExtendedFunction;
};

IMPLEMENTATION:

#include <stdio.h>
#include <VgaConsole.h>
#include <types.h>
#include INC_ARCH(EFLAGS.h)
#include <fatal.h>
#include <string.h>

/**
 * @brief Do the identification
 */
PUBLIC void CPUID::identify()
{
  if(!is486())
    fatal(ANSI_FG_RED "You need newer processor than 386 to run OOMTK\n");

  if (!supportsCPUID())
    fatal(ANSI_FG_RED "This CPU doesn't support CPUID instruction!!!\n");

  cpuidRegisters regs;

  // returns maximum eax number for calling the CPUID (= max function)
  maximumFunction = cpuid(0, regs);

  // swap ecx & edx for the vendor string "encoding"
  uint32_t tmp = regs.ecx;
  regs.ecx = regs.edx;
  regs.edx = tmp;
  const char * vendorString = reinterpret_cast<const char *>(&regs.ebx);
  memcpy(_cpuVendor, vendorString, 12);
  _cpuVendor[12] = '\0'; // Zero-terminate the string
  // swap ecx & edx back to normal
  tmp = regs.ecx;
  regs.ecx = regs.edx;
  regs.edx = tmp;

  if (strcmp(_cpuVendor, "GenuineIntel") == 0)
    _cpuVendorID = Intel;
  else if (strcmp(_cpuVendor, "AuthenticAMD") == 0)
    _cpuVendorID = AMD;
  else
    _cpuVendorID = Unknown;

  printf("CPU[%d]: vendor = %s\n", _cpuNumber, _cpuVendor);

  if (maximumFunction == 0)
    fatal("CPUID supports 0 functions, which is quite odd :(\n");

  // Function eax=1 returns information about family, model and stepping
  uint32_t cpuSignature = cpuid(1, regs);

  if (_cpuVendorID & (Intel | AMD))
  {
    _cpuFamily    = fields(regs.eax, 11, 8);
    _cpuModel     = fields(regs.eax,  7, 4);
    _cpuStepping  = fields(regs.eax,  3, 0);

    if ((_cpuFamily == 0xf) || (_cpuFamily == 0x6))
      _cpuModel   += fields(regs.eax, 19, 16) << 4;
    if (_cpuFamily == 0xf)
      _cpuFamily  += fields(regs.eax, 27, 20);

    _nLogicalProcessors = fields(regs.ebx, 23, 16);
    if (regs.edx & 0x10000000) // check for HyperThreading on AMD
      _nLogicalProcessors = 1;

    /**
     * @todo Somebody should read data from the documents and encode the models,
     * @todo families, steppings and so on as regular strings, not as these pure
     * @todo values...
     */
    printf("CPU[%d]: family %d, model %d, stepping %d, brand %d, clflush %d\n",
           _cpuNumber,
           _cpuFamily,
           _cpuModel,
           _cpuStepping,
           fields(regs.ebx, 7, 0),
           fields(regs.ebx, 15, 8)
          );
    printf("CPU[%d]: Logical CPUs: %d, Init APIC ID: %d\n",
           _cpuNumber,
           _nLogicalProcessors,
           fields(regs.ebx, 31, 24)
          );
  };

  // Get the extended information
  maximumExtendedFunction = cpuid(0x80000000, regs);
  if (maximumExtendedFunction >= 0x80000004) // processor name string is supported
  {
    // Get the first part of the string
    cpuid(0x80000002, regs);
    const char * nameString = reinterpret_cast<const char *>(&regs);
    memcpy(_cpuName, nameString, 16);
    // Get the second part of the string
    cpuid(0x80000003, regs);
    // nameString still points to regs structure :)
    memcpy(_cpuName + 16*sizeof(char), nameString, 16);
    // Get the third and last part of the string
    cpuid(0x80000004, regs);
    memcpy(_cpuName + 32*sizeof(char), nameString, 16);
    // Terminate the _cpuName string
    _cpuName[48] = '\0';
    // Print it on the console
    printf("CPU[%d]: name = %s\n", _cpuNumber, _cpuName);
  }
};

PROTECTED bool CPUID::is486()
{
  uint32_t i486;

  GNU_ASM(
      "   pushf                 \n" /* Push flags onto the stack    */
      "   popl  %%eax           \n" /* Get the original flags       */
      "   movl  %%eax, %%ecx    \n" /* Save the original flags      */
      "   xorl  %[ac], %%eax    \n" /* Flip the AC bit in flags     */
      "   pushl %%eax           \n" /* Put new flags on the stack   */
      "   popf                  \n" /* Replace current flags        */
      "   pushf                 \n" /* Get new flags                */
      "   popl  %%eax           \n" /* and store them in eax        */
      "   xorl  %%ecx, %%eax    \n" /* Compare new and old flags    */
      "   jz    1f              \n" /* new=old => 386               */
      "   movl  $1, %[i486]     \n" /* 486 = true                   */
      "   jmp   2f              \n" /* Go to the end of function    */
      "1: movl  $0, %[i486]     \n" /* 486 = false                  */
      "2: pushl %%ecx           \n" /* Restore the old flags        */
      "   popf                  \n"
  : [i486] "=m" (i486)
  : [ac] "i" (EFLAGS_AC)
  : "memory", "eax", "ecx"
         );

  return (i486)?true:false;
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

/**
 * @brief CPUID Constructor
 * @param cpuNumber the number of the CPU (if you have more than one CPU)
 * @param printOut print the messages on screen or not
 */
PUBLIC CPUID::CPUID(uint32_t cpuNumber = 0, bool printOut = true)
{
  // Construct local variables
  _cpuNumber = cpuNumber;
  _printOut = printOut;

  // Initialize the CPU features list
  // Please keep the features sorted by opcode, so that the identification algorithm
  // optimizations can be effective.

  // eax = 1, ecx features
  addFeature(SSE3,    0x00000001u, ECX, "SSE3",            0, Intel | AMD);
  addFeature(MONITOR, 0x00000001u, ECX, "MONITOR/MWAIT",   3, Intel);
  addFeature(DSCPL,   0x00000001u, ECX, "DS-CPL",          4, Intel);
  addFeature(VMX,     0x00000001u, ECX, "VMX",             5, Intel);
  addFeature(EST,     0x00000001u, ECX, "SpeedStep",       7, Intel);
  addFeature(TM2,     0x00000001u, ECX, "TM2",             8, Intel);
  addFeature(SSSE3,   0x00000001u, ECX, "SSSE3",           9, Intel);
  addFeature(CID,     0x00000001u, ECX, "CID",            10, Intel);
  addFeature(CX16,    0x00000001u, ECX, "CMPXCHG16B",     13, Intel | AMD);
  addFeature(xTPR,    0x00000001u, ECX, "xTPR",           14, Intel);
  addFeature(DCA,     0x00000001u, ECX, "DCA",            18, Intel);
  addFeature(RAZ,     0x00000001u, ECX, "RAZ",            31, AMD);
};

#include <stdarg.h>

/**
 * @brief Printf replacement that checks _printOut flag
 * @param message the printf format of message
 * @param ... the remaining arguments to printf
 * @returns the same int that printf returns
 */
PROTECTED int CPUID::printf(const char * message, ...)
{
  int n;
  va_list args;

  if (!_printOut)
    return 0;

  va_start(args, message);
  n = vprintf(message, args);
  va_end(args);

  return n;
}

/**
 * @brief Extract the fields from the word
 * @param word input word
 * @param hi upper field limit
 * @param lo lower field limit
 * @returns word[hi:lo] bits
 */
PROTECTED inline uint32_t CPUID::fields(uint32_t word, uint8_t hi, uint8_t lo)
{
  return (((2u << hi) - 1u) & word) >> lo;
}

/**
 * @brief Add the feature to the feature list
 * @param id id of the feature (hopefully less then sizeof(features) :) )
 * @param code the cpuid code that goes to eax
 * @param reg the register index (EAX, EBX, ECX or EDX)
 * @param name the name that should be printed on screen (sizeof(name) < 20)
 * @param bit the bit that defines the feature
 * @param vendor the bitfield of vendors that support this feature
 */
PROTECTED inline NEEDS [<string.h>]
void CPUID::addFeature(uint32_t id, uint32_t code, uint32_t reg, const char * name,
                       uint32_t bit, uint32_t vendor)
{
  features[id].code   = code;
  features[id].reg    = reg;
  strncpy(features[id].name, name, 20);
  features[id].name[19] = '\0'; // ensure that the string is null terminated
  features[id].bit    = bit;
  features[id].vendor = vendor;
}
