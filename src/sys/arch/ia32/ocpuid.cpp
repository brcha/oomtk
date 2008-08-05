/*
 *  Copyright (C) 2006-2008 by Filip Brcic <brcha@gna.org>
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

#include "ocpuid.h"

#include <cstdio>
#include <types.h>
#include INC_ARCH(EFLAGS.h)
#include <fatal.h>
#include <ansi.h>
#include <cstring>

#define DEBUG_CPUID DEBUG_ENABLE

OCPUID * OCPUID::instance()
{
  static OCPUID _instance = OCPUID();

  return &_instance;
};

void OCPUID::identify()
{
  if (_identified)
    return; // CPU has already been identified...

  cpuid_registers_t regs;

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

//   printf("CPU0: vendor = %s\n",  _cpuVendor);

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
     * \todo Somebody should read data from the documents and encode the models,
     * \todo families, steppings and so on as regular strings, not as these pure
     * \todo values...
     */
//     printf("CPU0: family %d, model %d, stepping %d, brand %d, clflush %d\n",
//            _cpuFamily,
//            _cpuModel,
//            _cpuStepping,
//            fields(regs.ebx, 7, 0),
//            fields(regs.ebx, 15, 8)
//           );
//     printf("CPU0: Logical CPUs: %d, Init APIC ID: %d\n",
//            _nLogicalProcessors,
//            fields(regs.ebx, 31, 24)
//           );
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
    DEBUG_CPUID printf("CPU0: %s\n", _cpuName);
  }

  // Check all the features and show results
  uint32_t code = 0;
  uint32_t currentLine = 0;
  bool firstPass = true;
  for (uint32_t i = 0; i < NUM_FEATURES; i++)
  {
    // Check if this features applies to current vendor
    if (!(features[i].vendor & _cpuVendorID))
      continue;

    // If this feature has a new unseen code, run CPUID again
    if (features[i].code != code)
    {
      // Check if this new code is supported
      if ((features[i].code < 0x80000000) &&
           (features[i].code > maximumFunction))
        continue; // unsupported standard function
      if ((features[i].code >= 0x80000000) &&
           (features[i].code > maximumExtendedFunction))
        continue; // unsupported extended function

      code = features[i].code;  // save the cpuid code
      cpuid(code, regs);        // and load new values into registers
    }

    // Now I have loaded CPUID registers for the function that is applicable to
    // current processor. I can check if feature is supported now.
    switch (features[i].reg)
    {
      case EAX:
        features[i].has = regs.eax & (1u << features[i].bit);
        break;
      case EBX:
        features[i].has = regs.ebx & (1u << features[i].bit);
        break;
      case ECX:
        features[i].has = regs.ecx & (1u << features[i].bit);
        break;
      case EDX:
        features[i].has = regs.edx & (1u << features[i].bit);
        break;
      default:
        fatal(ANSI_FG_RED "ERROR in OCPUID::identify(): features[%d].reg = %d\n",
              i, features[i].reg);
    }

    DEBUG_CPUID {
      if (features[i].has)
      {
	uint32_t len = strlen(features[i].name) + 1;
	if (firstPass)
	{
	  printf("CPU0: [ "); // 8 characters
	  firstPass = false;
	  currentLine = 11;
	}
	if ((currentLine + len) >= 75) // must write new line
	{
	  printf("\n        "); // 8 spaces
	  currentLine = 11;
	}
	printf("%s ", features[i].name);
	currentLine += len;
      }
    }
    printf("]\n");
  }

  _identified = true;
};

uint32_t OCPUID::cpuid(uint32_t code, cpuid_registers_t & regs)
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

OCPUID::OCPUID()
{
  // Construct local variables
  _identified = false;

  // Initialize the CPU features list

  // Features are sorted by opcode, so that the identification algorithm
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
  addFeature(CX16,    0x00000001u, ECX, "CX16",           13, Intel | AMD);
  addFeature(xTPR,    0x00000001u, ECX, "xTPR",           14, Intel);
  addFeature(DCA,     0x00000001u, ECX, "DCA",            18, Intel);
  addFeature(RAZ,     0x00000001u, ECX, "RAZ",            31, AMD);
  // eax = 1, edx features
  addFeature(FPU,     0x00000001u, EDX, "FPU",             0, Intel | AMD);
  addFeature(VME,     0x00000001u, EDX, "VME",             1, Intel | AMD);
  addFeature(DE,      0x00000001u, EDX, "DE",              2, Intel | AMD);
  addFeature(PSE,     0x00000001u, EDX, "PSE",             3, Intel | AMD);
  addFeature(TSC,     0x00000001u, EDX, "TSC",             4, Intel | AMD);
  addFeature(MSR,     0x00000001u, EDX, "MSR",             5, Intel | AMD);
  addFeature(PAE,     0x00000001u, EDX, "PAE",             6, Intel | AMD);
  addFeature(MCE,     0x00000001u, EDX, "MCE",             7, Intel | AMD);
  addFeature(CX8,     0x00000001u, EDX, "CX8",             8, Intel | AMD);
  addFeature(APIC,    0x00000001u, EDX, "APIC",            9, Intel | AMD);
  addFeature(SEP,     0x00000001u, EDX, "SEP",            11, Intel | AMD);
  addFeature(MTRR,    0x00000001u, EDX, "MTRR",           12, Intel | AMD);
  addFeature(PGE,     0x00000001u, EDX, "PGE",            13, Intel | AMD);
  addFeature(MCA,     0x00000001u, EDX, "MCA",            14, Intel | AMD);
  addFeature(CMOV,    0x00000001u, EDX, "CMOV",           15, Intel | AMD);
  addFeature(PAT,     0x00000001u, EDX, "PAT",            16, Intel | AMD);
  addFeature(PSE36,   0x00000001u, EDX, "PSE-36",         17, Intel | AMD);
  addFeature(PSN,     0x00000001u, EDX, "PSN",            18, Intel);
  addFeature(CLFSH,   0x00000001u, EDX, "CLFSH",          19, Intel | AMD);
  addFeature(DS,      0x00000001u, EDX, "DS",             21, Intel);
  addFeature(ACPI,    0x00000001u, EDX, "ACPI",           22, Intel);
  addFeature(MMX,     0x00000001u, EDX, "MMX",            23, Intel | AMD);
  addFeature(FXSR,    0x00000001u, EDX, "FXSR",           24, Intel | AMD);
  addFeature(SSE,     0x00000001u, EDX, "SSE",            25, Intel | AMD);
  addFeature(SSE2,    0x00000001u, EDX, "SSE2",           26, Intel | AMD);
  addFeature(SS,      0x00000001u, EDX, "SS",             27, Intel);
  addFeature(HTT,     0x00000001u, EDX, "HTT",            28, Intel | AMD);
  addFeature(iTM,     0x00000001u, EDX, "TM",             29, Intel);
  addFeature(IA64,    0x00000001u, EDX, "IA64",           30, Intel);
  addFeature(PBE,     0x00000001u, EDX, "PBE",            31, Intel);
  // eax = 80000001, ecx features
  addFeature(LahfSahf,  0x80000001u, ECX, "LahfSahf",      0, Intel | AMD);
  addFeature(CmpLegacy, 0x80000001u, ECX, "CmpLegacy",     1, AMD);
  addFeature(SVM,       0x80000001u, ECX, "SVM",           2, AMD);
  addFeature(AltMovCr8, 0x80000001u, ECX, "AltMovCr8",     4, AMD);
  // eax = 80000001, edx features
  addFeature(SYSCALL, 0x80000001u, EDX, "SYSCALL",        11, Intel | AMD);
  addFeature(NX,      0x80000001u, EDX, "NX",             20, AMD);   // AMD was first
  addFeature(XD,      0x80000001u, EDX, "XD",             20, Intel); // Intel's name
  addFeature(MmxExt,  0x80000001u, EDX, "MmxExt",         22, AMD);
  addFeature(FFXSR,   0x80000001u, EDX, "FFXSR",          25, AMD);
  addFeature(RDTSCP,  0x80000001u, EDX, "RDTSCP",         27, AMD);
  addFeature(LM,      0x80000001u, EDX, "LM",             29, AMD);   // AMD64
  addFeature(Intel64, 0x80000001u, EDX, "Intel64",        29, Intel); // Intel64
  addFeature(a3DNowExt, 0x80000001u, EDX, "3DNow!Ext",    30, AMD);
  addFeature(a3DNow,  0x80000001u, EDX, "3DNow!",         31, AMD);
  // eax = 80000007, edx features (AMD only)
  addFeature(TS,      0x80000007u, EDX, "TS",              0, AMD);
  addFeature(FID,     0x80000007u, EDX, "FID",             1, AMD);
  addFeature(VID,     0x80000007u, EDX, "VID",             2, AMD);
  addFeature(TTP,     0x80000007u, EDX, "TTP",             3, AMD);
  addFeature(aTM,     0x80000007u, EDX, "TM",              4, AMD);
  addFeature(STC,     0x80000007u, EDX, "STC",             5, AMD);
  addFeature(TscInvariant, 0x80000007u, EDX, "TscInvariant", 8, AMD);
};

inline uint32_t OCPUID::fields(uint32_t word, uint8_t hi, uint8_t lo)
{
  return (((2u << hi) - 1u) & word) >> lo;
}

void OCPUID::addFeature(uint32_t id, uint32_t code, uint32_t reg, const char * name,
                       uint32_t bit, uint32_t vendor)
{
  features[id].code   = code;
  features[id].reg    = reg;
  strncpy(features[id].name, name, 20);
  features[id].name[19] = '\0'; // ensure that the string is null terminated
  features[id].bit    = bit;
  features[id].vendor = vendor;
}
