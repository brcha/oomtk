#ifndef __IA32_OACPI_H__
#define __IA32_OACPI_H__
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
/** @file oacpi.h
 * @brief Minimal ACPI support
 */

#include <types.h>
#include <cstddef>

#define DEBUG_ACPI DEBUG_ENABLE

/**
 * @brief Minimal ACPI support
 */
class OACPI
{
  public:
    static OACPI * instance();
    
    // Structures
    struct rsdp_v3_t
    {
      char      signature[8];
      uint8_t   cksum;
      char      oemid[6];
      uint8_t   revision;
      uint32_t  rsdt;         // physical address of RSDT
      uint32_t  length;       // length of RSDT
      
      // Fields below are from ACPI v3.0
      uint64_t  xsdt;         // pa of XSDT
      uint8_t   ex_cksum;     // extended checksum
      char      reserved[3];
    };
    
    struct sdt_header_t
    {
      char      signature[4];
      uint32_t  length;
      uint8_t   revision;
      uint8_t   cksum;
      char      oemid[6];
      char      oemTableID[8];
      uint32_t  oemRevision;
      uint32_t  creatorID;
      uint32_t  creatorRevision;
    };
    
    enum LAPIC {
      ProcessorLocalAPIC  = 0x0,
      IO_APIC             = 0x1u,
      IntrSourceOverride  = 0x2u,
      NMI_Source          = 0x3u,
      LAPIC_NMI_Struct    = 0x4u,
      LAPICAddrOverride   = 0x5u,
      IO_SAPIC            = 0x6u,
      LocalSAPIC          = 0x7u,
      PlatformIntrSource  = 0x8u,
      
      Enabled             = 0x1u, // CPU enabled/disabled
    };
    
    struct cpu_local_apic_t
    {
      uint8_t   type;
      uint8_t   length;
      uint8_t   acpiCpuID;
      uint8_t   lapicID;
      uint32_t  flags;
    };
    
    struct cpu_io_apic_t
    {
      uint8_t   type;
      uint8_t   length;
      uint8_t   ioapicID;
      uint8_t   reserved;
      uint32_t  ioapicPA;
      uint32_t  globalSystemInterruptBase;
    };
    
    enum {
      AcpiBusIsa  = 0,
    };
    
    enum MPS {
      MpsIntiPolarity           = 0x0003u,
      MpsIntiPolarityFromBus    = 0x00,
      MpsIntiPolarityActiveHi   = 0x01,
      MpsIntiPolarityActiveLo   = 0x03,
      MpsIntiTrigger            = 0x000cu,
      MpsIntiTriggerFromBus     = 0x00,
      MpsIntiTriggerEdge        = 0x04,
      MpsIntiTriggerLevel       = 0x0c,
    };
    
    struct int_src_override_t
    {
      uint8_t   type;
      uint8_t   length;
      uint8_t   bus;        // 0 => ISA
      uint8_t   source;     // bus relative irq
      uint32_t  globalSystemInterrupt;
      uint16_t  flags;      // mps inti flags
    };
    
    /**
     * @brief Populate CPU structures for available CPUs
     * @returns the number of CPUs (according to ACPI) or 0 if information is not available.
     */
    size_t probeCpus();
    
    /**
     * @brief Probe ACPI tables for IRQ sources and initialize vector table if found
     * @returns true if ACPI interrupt sources are found.
     */
    bool probeAcpis();
    
    /**
     * @brief Find out where ACPI mapped a given interrupt
     */
//     bool mapInterrupt(irq_t irq, int_src_override_t * ovr);
  protected:
    OACPI();
    ~OACPI();
    
    uint8_t cksum (kpa_t pa, size_t len);
    
    kpa_t scanRegion (kpa_t ptr, kpa_t bound);
    
    /**
     * @brief Locate Multiple APIC Description Table
     */
    kpa_t findMadt();
};

#endif /* __IA32_OACPI_H__ */
