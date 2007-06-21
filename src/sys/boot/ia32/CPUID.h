#ifndef __CPUID_H__
#define __CPUID_H__
/*
 *  Copyright (C) 2006-2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file CPUID.h
 * \brief CPU Identification
 */
/*
 * $Id$
 */

#include <types.h>

/**
 * \brief CPUID class identifies the CPU
 *
 * For more info on CPUID for Intel and AMD see
 * \ref ia32v2a "Intel's instruction set reference",
 * \ref ia32v3a "Intel's system programming guide",
 * \ref ia32ap485 "Intel's processor identification note",
 * \ref amd64v1 "AMD's application programming guide",
 * \ref amd64v2 "AMD's system programming guide",
 * \ref amd64v3 "AMD's general purpose instructions",
 * \ref AMDrecognition "AMD's processor recognition note" and
 * \ref AMDcpuid "AMD's CPUID specification".
 */
class CPUID
{
  public:
    /**
     * \brief CPU Vendor
     *
     * These values should form a bitfield, so they should be 2^n
     */
    enum cpuVendor_t
    {
      Unknown   = 0x0u,
      Intel     = 0x1u,
      AMD       = 0x2u,
    };

    /**
     * \brief CPU Features
     *
     * Names of the features are the same as in IA-32 manual (with excluded '-'
     * in some names and with 'a' added in front of AMD's 3DNow features).
     *
     * For Intel see \ref ia32ap485 "AP-485, Intel Processor Identicifation and the
     * CPUID Instruction" and \ref ia32v2a "Intel64 and IA-32 Intel Architecture
     * Software Developer's Manual, Volume 2A: Instruction Set Reference, A-M, pages
     * 160-188".
     *
     * For AMD see \ref AMDcpuid "AMD's CPUID Specification" and \ref amd64v3 "AMD64
     * Architecture Programmer's Manual, Volume 3: General-Purpose and System
     * Instructions, pages 102-103".
     */
    enum cpuFeature_t
    {
      // eax = 1, ecx features
      SSE3 = 0,   ///< \brief Streaming SIMD Extensions 3 (SSE3)
      MONITOR,    ///< \brief Monitor/MWait support
      DSCPL,      ///< \brief CPL Qualified Debug Store
      VMX,        ///< \brief Virtual Machine Extensions
      EST,        ///< \brief Enhanced Intel SpeedStep technology
      TM2,        ///< \brief Thermal Monitor 2
      SSSE3,      ///< \brief Supplemental Streaming SIMD Extensions 3
      CID,        ///< \brief L1 Context ID
      CX16,       ///< \brief CMPXCHG16B instruction supported
      xTPR,       ///< \brief Send Task Priority Messages
      DCA,        ///< \brief Direct Cache Access
      RAZ,        ///< \brief AMD64 64-bit register parts access
      // eax = 1, edx features
      FPU,        ///< \brief Floating Point Unit On-Chip
      VME,        ///< \brief Virtual 8086 Mode Enhancements
      DE,         ///< \brief Debugging Extensions - I/O breakpoints, CR4.DE, ...
      PSE,        ///< \brief Page Size Extension - large 4 MB pages
      TSC,        ///< \brief Time Stamp Counter - CR4.TSD & RDTSC instruction
      MSR,        ///< \brief Model Specific Registers - {RD,WR}MSR
      PAE,        ///< \brief Physical Address Extension - address > 32bits
      MCE,        ///< \brief Machine Check Exception - exc.18
      CX8,        ///< \brief CMPXCHG8B instruction supported
      APIC,       ///< \brief APIC On-Chip (Advanced PIC)
      SEP,        ///< \brief SYSENTER & SYSEXIT instructions
      MTRR,       ///< \brief Memory Type Range Registers
      PGE,        ///< \brief PTE Global Bit
      MCA,        ///< \brief Machine Check Architecture
      CMOV,       ///< \brief Conditional Move instruction supported
      PAT,        ///< \brief Page Attribute Table
      PSE36,      ///< \brief 36bit Page Size Extension - 4 MB pages beyond 4 GB
      PSN,        ///< \brief Processor Serial Number (96bit)
      CLFSH,      ///< \brief CLFLUSH instruction
      DS,         ///< \brief Debug Store
      ACPI,       ///< \brief Thermal Monitor and Software Controlled Clock Facilities
      MMX,        ///< \brief Intel MMX Technology
      FXSR,       ///< \brief FXSAVE and FXRSTOR instructions (for FPU context)
      SSE,        ///< \brief SSE extensions
      SSE2,       ///< \brief SSE2 extensions
      SS,         ///< \brief Self Snoop
      HTT,        ///< \brief Multi-Threading - more than one logical processor
      iTM,        ///< \brief Thermal Monitor (Intel)
      IA64,       ///< \brief Intel IA-64 capabilities (=> this is IA32e mode)
      PBE,        ///< \brief Pending Break Enable
      // eax = 80000001, ecx features
      LahfSahf,   ///< \brief LAHF/SAHF available in 64bit mode
      CmpLegacy,  ///< \brief CMPLEGACY available (Athlon64 and Opteron)
      SVM,        ///< \brief AMD's Secure Virtual Machine feature
      AltMovCr8,  ///< \brief LOCK MOV CR0 means MOV CR8
      // eax = 80000001, edx features
      SYSCALL,    ///< \brief SYSCALL and SYSRET instructions
      NX,         ///< \brief No-eXecute page protection (AMD)
      XD,         ///< \brief eXecution Disable bit (Intel)
      MmxExt,     ///< \brief AMD's extensions to MMX instructions
      FFXSR,      ///< \brief FXSAVE and FXRSTOR instruction optimizations
      RDTSCP,     ///< \brief RDTSCP instruction
      LM,         ///< \brief AMD's Long Mode (64-bit mode)
      Intel64,    ///< \brief Intel's EM64T mode (64-bit mode)
      a3DNowExt,  ///< \brief AMD's extensions for 3DNow! instructions
      a3DNow,     ///< \brief 3DNow! instructions
      // eax = 80000007, edx features (APM information on AMD)
      TS,         ///< \brief Temperature sensor
      FID,        ///< \brief Frequency ID control
      VID,        ///< \brief Voltage ID control
      TTP,        ///< \brief THERMTRIP is supported
      aTM,        ///< \brief Hardware thermal control is supported (AMD)
      STC,        ///< \brief Software thermal control is supported
      TscInvariant, ///< \brief TSC rate is ensured to be invariant across all P,C-States and stop-grant transitions (such as STPCLK Throttling) => TSC is good as a time source.
      NUM_FEATURES  ///< \brief This is for size of features list
    };

  protected:
    /// \brief Some constants used in the code
    enum
    {
      // Registers used in features definition list
      EAX   = 0,
      EBX   = 1,
      ECX   = 2,
      EDX   = 3,
    };

    /**
     * \brief Structure that defines the features of this CPU
     */
    struct features_t
    {
      uint32_t  code;
      uint32_t  reg;
      char name[20]; // maximum name of feature is 19 characters ( + '\0')
      uint32_t  bit;
      uint32_t  vendor;
      bool      has;
    };

    /**
     * \brief Features of this CPU
     */
    features_t features[NUM_FEATURES];

    /// \brief Registers for the cpuid() function
    struct cpuidRegisters
    {
      uint32_t eax;
      uint32_t ebx;
      uint32_t ecx;
      uint32_t edx;
    };

  public:
    /**
     * \brief Make only one instance of this class
     * \returns the instance
     */
    static CPUID * instance(bool printOut = true);

    /**
     * \brief Do the identification
     */
    void identify();

    // inline get/set methods
    /**
     * \brief Does the CPU have the feature?
     * \param feature CPU Feature in question
     * \returns true if CPU supports the feature
     */
    inline bool has(cpuFeature_t feature)
    {
      if (feature < NUM_FEATURES)
        return features[feature].has;
      return false;
    }

    /**
     * \brief Is CPU identified?
     */
    inline bool isIdentified()
    {
      return _identified;
    }

    /**
     * \brief What is the CPU name?
     * \returns CPU name
     */
    inline const char * name()
    {
      if (!_identified)
        return "";
      return _cpuName;
    }

    /**
     * \brief What is the CPU vendor?
     * \returns vendor string
     */
    inline const char * vendor()
    {
      if (!_identified)
        return "";
      return _cpuVendor;
    }

    /**
     * \brief What is the CPU vendor?
     * \returns vendor id
     */
    inline uint32_t vendorId()
    {
      if (!_identified)
        return 0;
      return _cpuVendorID;
    }

  protected:
    bool is486();
    bool supportsCPUID();

    /**
     * \brief Calls the CPUID function
     * \param code cpuid code (-> eax)
     * \param regs returned cpuid registers
     * \returns contents of eax
     */
    inline uint32_t cpuid(uint32_t code, cpuidRegisters & regs);

    /**
     * \brief CPUID Constructor
     * \param printOut print the messages on screen or not
     */
    CPUID(bool printOut = true);

    /**
     * \brief Printf replacement that checks _printOut flag
     * \param message the printf format of message
     * \param ... the remaining arguments to printf
     * \returns the same int that printf returns
     */
    int printf(const char * message, ...);

    /**
     * \brief Extract the fields from the word
     * \param word input word
     * \param hi upper field limit
     * \param lo lower field limit
     * \returns word[hi:lo] bits
     */
    inline uint32_t fields(uint32_t word, uint8_t hi, uint8_t lo);

    /**
     * \brief Add the feature to the feature list
     * \param id id of the feature (hopefully less then sizeof(features) :) )
     * \param code the cpuid code that goes to eax
     * \param reg the register index (EAX, EBX, ECX or EDX)
     * \param name the name that should be printed on screen (sizeof(name) < 20)
     * \param bit the bit that defines the feature
     * \param vendor the bitfield of vendors that support this feature
     */
    void addFeature(uint32_t id, uint32_t code, uint32_t reg, const char * name,
                    uint32_t bit, uint32_t vendor);

  protected:
    /// \brief CPU Vendor String ("GenuineIntel" or "AuthenticAMD")
    char _cpuVendor[13];

    /// \brief CPU Name String (functions 0x8000000[4:2], eax, ebx, ecx & edx)
    char _cpuName[49];

    /// \brief CPU Vendor ID (from cpuVendor_t enum)
    cpuVendor_t _cpuVendorID;

    // eax = 1, eax fields
    uint32_t _cpuFamily;

    uint32_t _cpuModel;

    uint32_t _cpuStepping;

    // eax = 1, ebx fields
    uint32_t _nLogicalProcessors;

    /// \brief Print to console or not?
    bool _printOut;

    /// \brief Maximum function
    uint32_t maximumFunction;

    /// \brief Maximum extended function
    uint32_t maximumExtendedFunction;

    /// \brief True when CPU has been identified
    bool _identified;
};

#endif /* __CPUID_H__ */
