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
#include "oacpi.h"

#include <ia32/pagesize.h>

#include <OOMTK/OPaging>
#include <OOMTK/OPhysicalMemoryManagement>
#include <OOMTK/OCPU>

#include <cstdio>
#include <cstring>
#include <cassert>
#include <vpstring.h>
#include <fatal.h>

#include "ocpuid.h"

OACPI::OACPI()
{
}


OACPI::~OACPI()
{
}

OACPI * OACPI::instance()
{
  static OACPI oacpi = OACPI();
  
  return &oacpi;
}

uint8_t OACPI::cksum(kpa_t pa, size_t len)
{
  uint8_t sum = 0;
  
  while (len)
  {
    kpa_t offset = pa & OOMTK_PAGE_ADDR_MASK;
    pa -= offset;
    
    char * va = (char *) OPaging::instance()->map(pa);
    char * ptr = va + offset;
    kpa_t count = min(len, OOMTK_PAGE_SIZE - offset);
    
    for (size_t i = 0; i < count; i++)
      sum += ptr[i];
    
    OPaging::instance()->unmap((kva_t) va);
    
    len -= count;
  }
  
  return sum;
}

kpa_t OACPI::scanRegion(kpa_t ptr, kpa_t bound)
{
  while (ptr < bound)
  {
    rsdp_v3_t * rsdp_va = PTOKV(ptr, rsdp_v3_t *);
    kpa_t rsdp_pa = ptr;
    ptr += 16;
    
    if (memcmp ((char *) rsdp_va->signature, "RSD PTR ", 8) != 0)
      continue;
    
    // Test the v1.0 checksum
    if (cksum (rsdp_pa, 20))
      continue;
    
    // Test the v3.0 checksum
    if ((rsdp_va->revision >= 2) && cksum(rsdp_pa, rsdp_va->length))
      continue;
    
    // Checksums ok, check if rsdt pointer points at an rsdt structure by testing signature
    char rsdt_sig[4];
    memcpy_ptov(&rsdt_sig, rsdp_va->rsdt, 4);
    if (memcmp(rsdt_sig, "RSDT", 4) != 0)
      continue;
    
    // Found RSDP :)
    return rsdp_pa;
  }
  
  return 0;
}

kpa_t OACPI::findMadt()
{
  static kpa_t madt_pa = 0;
  
  if (madt_pa)
    return madt_pa;
  
  // First find RSDP. ACPI specification says that it must be within first 1024 of EBDA, or
  // in BIOS ROM between E0000 and FFFFF.
  // These addresses are in low 2MB, so they are already mapped.
  kpa_t ebda = ( (* PTOKV(0x40e, uint16_t *)) << 4);
  kpa_t rsdp = scanRegion(ebda, ebda + 1024);
  if (! rsdp)
    rsdp = scanRegion(0xe0000, 0xfffff);
  if (! rsdp)
    return 0;
  
  DEBUG_ACPI printf("ACPI RSDP found at " OOMTK_KPA_FMT "\n", rsdp);
  
  // ACPI RSDP is found. Therefore we know location of RSDT. Now find MADT
  kpa_t rsdt_pa = PTOKV(rsdp, rsdp_v3_t *)->rsdt;
  sdt_header_t madt_hdr;
  sdt_header_t rsdt_hdr;
  
  memcpy_ptov (&rsdt_hdr, rsdt_pa, sizeof(rsdt_hdr));
  
  if (cksum(rsdt_pa, rsdt_hdr.length))
    fatal ("ACPI checksum on RSDT failed!\n");
  
  size_t entries = (rsdt_hdr.length - sizeof(rsdt_hdr)) / 4;
  
  DEBUG_ACPI printf("ACPI RSDT " OOMTK_KPA_FMT " %d entries=%d\n", rsdt_pa, rsdt_hdr.length, entries);
  
  for (size_t i = 0; i < entries; i++)
  {
    kpa_t pa;
    uint32_t pa32;
    
    memcpy_ptov(&pa32, rsdt_pa + sizeof(rsdt_hdr) + (4*i), sizeof(uint32_t));
    pa = pa32;
    
    memcpy_ptov(&madt_hdr, pa, sizeof(madt_hdr));
    
    DEBUG_ACPI printf("ACPI %c%c%c%c " OOMTK_KPA_FMT " %d.\n",
	madt_hdr.signature[0],
	madt_hdr.signature[1],
        madt_hdr.signature[2],
	madt_hdr.signature[3],
        pa, madt_hdr.length);
    
    if (cksum(pa, madt_hdr.length))
    {
      printf("  [ checksum failed ]\n");
    }
    else if (memcmp(madt_hdr.signature, "APIC", 4) == 0)
    {
      // Some BIOSes have been obsered in the wild with multiple APIC tables
      if (!madt_pa) madt_pa = pa;
    }
  }
  
  return madt_pa;
}

bool OACPI::probeAcpis()
{
  kpa_t madt_pa = findMadt();
  // TODO: lapic_pa,_va shouldn't be here, they should be in LAPIC class or something...
  kpa_t lapic_pa;
  kva_t lapic_va;
  
  if (madt_pa == 0)
    return false;
  
  memcpy_ptov(&lapic_pa, madt_pa + sizeof(sdt_header_t), sizeof(uint32_t));
  
  assert (lapic_pa != 0);
  OPhysicalMemoryManagement * pmm = OPhysicalMemoryManagement::instance();
  pmm->allocRegion(lapic_pa, lapic_pa + 4096, pmm->cls_RAM, pmm->use_Kernel, "LAPIC");
  OPaging * paging = OPaging::instance();
  paging->kmap(IA32_LOCAL_APIC_VA, lapic_pa, OOMTK_PAGE_SIZE,
	       OPaging::KMAP_R|OPaging::KMAP_W|OPaging::KMAP_NC, false);
  lapic_va = IA32_LOCAL_APIC_VA;
  
  uint32_t multipleApicFlags;
  memcpy_ptov(&multipleApicFlags, madt_pa + sizeof(sdt_header_t) + 4, sizeof(uint32_t));
  if (multipleApicFlags & 0x1u)
  {
    printf("8259 must be disabled on lapic start.\n");
    // lapic_requires_8259_disable = true;
  }
  
  sdt_header_t madt_hdr;
  memcpy_ptov(&madt_hdr, madt_pa, sizeof(madt_hdr));
  
  kpa_t madt_bound = madt_pa + madt_hdr.length;
  kpa_t apicstruc_pa = madt_pa + sizeof(madt_hdr) + 8;
  
  cpu_io_apic_t ioapic;
  
  bool found = false;
  
  size_t nIoAPIC = 0;
  
  // Issue: ACPI doesn't specify ordering for entries in the APIC table.
  for (kpa_t pa = apicstruc_pa; pa < madt_bound; pa += ioapic.length)
  {
    memcpy_ptov (&ioapic, pa, 2);
    
    assert (ioapic.length);
    
    if (ioapic.type == IO_APIC)
    {
      memcpy_ptov(&ioapic, pa, sizeof(ioapic));
      
      kpa_t ioapic_pa = ioapic.ioapicPA;
      assert (ioapic_pa != 0);
      pmm->allocRegion(ioapic_pa, ioapic_pa + 4096, pmm->cls_RAM, pmm->use_Kernel, "IO APIC");
      
      kva_t va = IA32_IO_APIC_VA + (nIoAPIC * OOMTK_PAGE_SIZE);
      paging->kmap(va, ioapic_pa, OOMTK_PAGE_SIZE,
		   OPaging::KMAP_R|OPaging::KMAP_W|OPaging::KMAP_NC, false);
      
      // TODO: ioapic_register(ioapic.globalSystemInterruptBase, va);
      
      printf("IO APIC id=%02x at %08x intBase %d\n",
	     ioapic.ioapicID, ioapic.ioapicPA, ioapic.globalSystemInterruptBase);
      
      found = true;
    }
  }
  
  return found;
}

#include <ia32/archcpu.h>
extern arch_cpu_t archcpu_vector[MAX_NCPU];

size_t OACPI::probeCpus()
{
  size_t ncpu = 0;
  uint32_t madt_pa = findMadt();
  
  if (madt_pa == 0)
    return ncpu;
  
  OCPUID::cpuid_registers_t regs;
  (void) OCPUID::instance()->cpuid(1u, regs);
  uint8_t bootApicID = FIELD(regs.ebx, 31, 24);
  
  sdt_header_t madt_hdr;
  memcpy_ptov(&madt_hdr, madt_pa, sizeof(madt_hdr));
  
  kpa_t madt_bound = madt_pa + madt_hdr.length;
  kpa_t apicstruc_pa = madt_pa + sizeof(madt_hdr) + 8;
  
  cpu_local_apic_t lapic;
  
  // Issue: ACPI doesn't specify the ordering of the entries in the APIC table.
  ncpu = 1;
  for (kpa_t pa = apicstruc_pa; pa < madt_bound; pa += lapic.length)
  {
    memcpy_ptov(&lapic, pa, 2);
    
    assert (lapic.length);
    
    if (lapic.type == ProcessorLocalAPIC)
    {
      memcpy_ptov(&lapic, pa, sizeof(lapic));
      
      if ((lapic.flags & Enabled) == 0)
	continue;
      
      size_t slot = ncpu;
      if (lapic.lapicID == bootApicID)
	slot = 0;
      
      archcpu_vector[slot].lapic_id = lapic.lapicID;
      OCPU::m_vector[slot].m_present = true;
      
      if (slot != 0)
	ncpu ++;
    }
  }
  
  return ncpu;
}


