/*
 *  Copyright (C) 2006-2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file
 * \brief Paging for PAE enabled processors
 */
/*
 * $Id$
 */

#include "PagingPAE.h"

#include <stdio.h>
#include <ansi.h>
#include <fatal.h>
#include <CPUID.h>
#include INC_ARCH(CR.h)

/**
 * \brief Get the one instance of paging class
 * \returns the instance
 */
PagingPAE * PagingPAE::instance()
{
  static PagingPAE paging = PagingPAE();

  return &paging;
};

/**
 * \defgroup Mappings
 * \brief Mappings as declared in linker.ia32.ld.S + Kernel's PDPT
 * @{
 */

/**
 * \brief Kernel's PDPT
 *
 * Must be declared in the .data.percpu because of SMP. Like this it is aligned to
 * CPU cache size, so the CPUs could easily cache only what they need.
 *
 * This approach comes from Coyotos, and I hope it will work nicely (Coyotos doesn't
 * support SMP right now and neither does OOMTK, but I hope to support it as soon as
 * possible, and I found out that QEmu can emulate more than one CPU, so I will be
 * able to test it).
 */
DEFINE_PER_CPU(uint64_t[4], KernPDPT);

extern void * MasterPageDir;  ///< \brief Master paging directory
extern void * KernPageTable;  ///< \brief Kernel paging table
extern void * TransMap;       ///< \brief Transient mappings page

/** @} */ // Mappings

/**
 * \brief Protected constructor
 */
PagingPAE::PagingPAE() :
    Paging()
{
  KernelPDPT        = reinterpret_cast<PDPTE_t*>(&KernPDPT);
  PageDirectory     = reinterpret_cast<PDE_t*>  (&MasterPageDir);
  KernelPageTable   = reinterpret_cast<PTE_t*>  (&KernPageTable);
  TransientMappings = reinterpret_cast<PTE_t*>  (&TransMap);
};

/**
 * \brief Setup paging
 *
 * After booting, the kernel is located at physical memory location 0x00100000,
 * but kernel is relocated at 0xC0100000 (= KVA + 0x00100000 = 3 GB + 1 MB).
 *
 * This method should map the physical memory [0, 2M] to virtual [0, 2M] and also
 * to [KVA, KVA + 2M].
 *
 * PT and PD have 512 entries.
 */
void PagingPAE::setup()
{
  DEBUG_PAGING
      printf("Starting IA32 PAE paging ... ");

  // Map first [0, 2M]
  uint64_t address = 0u;
  for (size_t i = 0; i < 512; i++)
  {
    // set the bits
    KernelPageTable[i].value            = address;
    KernelPageTable[i].bits4k.present   = 1;
    KernelPageTable[i].bits4k.rw        = 1;
    KernelPageTable[i].bits4k.accessed  = 1;
    KernelPageTable[i].bits4k.dirty     = 1;
    // update the address for the next run
    address += 0x1000; // = 4 kb
  }

  // Set the paging directory to point to kernel's page table

  // First set the first 0 - 2 Mb
  PageDirectory[0].value            =
      reinterpret_cast<uint32_t>(&KernelPageTable[0]) - KVA;
  PageDirectory[0].bits4k.present   = 1;
  PageDirectory[0].bits4k.rw        = 1;
  PageDirectory[0].bits4k.accessed  = 1;
  // 4 kb page - not needed, but clarifies the code
  PageDirectory[0].bits4k.ps        = 0;

  // Put the transient mappings to the top 12 Mb
  // Here that is entry no 0xFD0
  PageDirectory[0xFD0].value          =
      reinterpret_cast<uint32_t>(&TransientMappings[0]) - KVA;
  PageDirectory[0xFD0].bits4k.present = 1;
  PageDirectory[0xFD0].bits4k.rw      = 1;

  // Now set up the PDPT <- this relies on KVA = 0xC0000000, so I will not change that
  KernelPDPT[0].value         =
      reinterpret_cast<uint32_t>(&PageDirectory[0]) - KVA;
  KernelPDPT[0].bits.present  = 1;
  KernelPDPT[1].value         = 0u;
  KernelPDPT[2].value         = 0u;
  KernelPDPT[3].value         =
      reinterpret_cast<uint32_t>(&PageDirectory[0]) - KVA;
  KernelPDPT[3].bits.present  = 1;

  // Load up KernelPDPT into CR3
  PDPT_Register_t CR3;
  CR3.value = reinterpret_cast<uint32_t>(&KernelPDPT[0]) - KVA;
  //CR3.bits.no need to setup anything
  cr3(CR3.value);

  // Setup the CR4
  uint32_t CR4 = cr4();

  CPUID * cpuid = CPUID::instance();

  cpuid->identify(); // identify the CPU if not already identified
  if (!cpuid->has(CPUID::PAE)) // why was this class called if CPU doesn't have PAE?
    // Halt the CPU
    fatal(ANSI_FG_RED "[failed]\n" ANSI_NORMAL);
  CR4 |= CR4_PAE;
  if (cpuid->has(CPUID::PSE)) // Page Size Extension
      CR4 |= CR4_PSE;
  if (cpuid->has(CPUID::PGE)) // Page Global Extension
      CR4 |= CR4_PGE;

  // store the new value of CR4
  cr4(CR4);

  // Now enable paging
  uint32_t CR0 = cr0();

  CR0 |= CR0_PG;  // paging
  CR0 |= CR0_AM;  // alignment mask
  CR0 |= CR0_WP;  // write protect
  CR0 |= CR0_ET;  // extension type
  CR0 |= CR0_TS;  // task switched
  CR0 |= CR0_MP;  // monitor coprocessor
  CR0 |= CR0_PE;  // protection enable (should have been enabled by grub)

  cr0(CR0);

  DEBUG_PAGING
      printf(ANSI_FG_GREEN "[ok]\n" ANSI_NORMAL);
};
