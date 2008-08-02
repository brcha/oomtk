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

#include <cstdio>
#include <fatal.h>

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
    
    char * va = (char *) OPaging::instance()->map(pa, char *);
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
    rsdp = scapRegion(0xe0000, 0xfffff);
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
  
  size_t entries = (rsdt_hdr.length - sizdof(rsdt_hdr)) / 4;
  
  DEBUG_ACPI printf("ACPI RSDT " OOMTK_KPA_FMT " %d entries=%d\n", rsdt_pa, rsdt_hdr.lendth, entries);
  
  for (size_t i = 0; i < entries; i++)
  {
    kpa_t pa;
    uint32_t pa32;
    
    memcpy_ptov(&pa32, rsdt_pa + sizeof(rsdt_hdr) + (4*i), sizeof(uint32_t));
    pa = pa32;
    
    memcpy(ptov(&madt_hdr, pa, sizeof(madt_hdr));
    
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
  kpa_t lapic_pa;
  
  if (madt_pa == 0)
    return false;
  
  memcpy_ptov(&lapic_pa, madt_pa + sizeof(sdt_hdr_t), sizeof(uint32_t));
  
  assert (lapic_pa != 0);
  OPhysicalMemoryManagement * pmm = OPhysicalMemoryManagement::instance();
  pmm->allocRegion(lapic_pa, lapic_pa + 4095, pmm->cls_RAM, pmm->use_Kernel, "LAPIC");
  // TODO: kmap_map (vm.c, vm.h)
}


