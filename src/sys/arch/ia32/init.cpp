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

#include <cstddef>
#include <cstdio>
#include <cassert>

#include <ansi.h>

#include <multiboot.h>

#include <ia32/pagesize.h>

#include <OOMTK/OVgaConsole>
#include <OOMTK/OCPU>
#include <OOMTK/Version>
#include <OOMTK/OPhysicalMemoryManagement>
#include <OOMTK/OCommandLine>

#include "ogdt.h"
#include "otss.h"
#include "opaging.h"
#include "opagingpae.h"
#include "opaginglegacy.h"

extern "C" {
  uint32_t multibootSignature;  ///< @brief Multiboot signature
  uint32_t multibootInfo;       ///< @brief Multiboot information pointer
  
  // Locations from ldscript.S:
  extern uint32_t _start[];
  extern uint32_t _text[];
  extern uint32_t _etext[];
  extern uint32_t _rodata[];
  extern uint32_t _erodata[];
  extern uint32_t __syscall_page[];
  extern uint32_t __esyscall_page[];
  extern uint32_t __begin_maps[];
  extern uint32_t __end_maps[];
  extern uint32_t cpu0_kstack_lo[];
  extern uint32_t cpu0_kstack_hi[];
  extern uint32_t _pagedata[];
  extern uint32_t _epagedata[];
  extern uint32_t _data[];
  extern uint32_t _edata[];
  extern uint32_t _bss_start[];
  extern uint32_t _bss_end[];
  extern uint32_t _end[];
}

OPhysicalMemoryManagement::Class mmapToClass(uint32_t multibootType)
{
  switch (multibootType)
  {
    case 1:
      return OPhysicalMemoryManagement::cls_RAM;
    case 2:
      return OPhysicalMemoryManagement::cls_ROM;
    case 3:
      return OPhysicalMemoryManagement::cls_NVRAM;
    case 4:
      return OPhysicalMemoryManagement::cls_RAM;
    default:
      assert(false);
  }
}

OPhysicalMemoryManagement::Use mmapToUse(uint32_t multibootType)
{
  switch (multibootType)
  {
    case 1:
      return OPhysicalMemoryManagement::use_Available;
    case 2:
      return OPhysicalMemoryManagement::use_BIOS;
    case 3:
      return OPhysicalMemoryManagement::use_ACPI_NVS;
    case 4:
      return OPhysicalMemoryManagement::use_ACPI_DATA;
    default:
      assert(false);
  }
}

/**
 * @brief Initialize physical memory.
 * 
 * @warning This method is multiboot specific!
 */
void configurePhysicalMemory(void)
{
  // On entry, only the low 1G (0x40000000) if mapped!
  
  if (multibootSignature != MULTIBOOT_BOOTLOADER_MAGIC)
    fatal("Multiboot signature invalid: %08x\n", multibootSignature);
  
  assert(multibootInfo < 0x40000000u);
  assert(multibootInfo + sizeof(struct multiboot_info_t) < 0x40000000u);
  
  struct multiboot_info_t * mbi = PTOKV(multibootInfo, struct multiboot_info_t *);
  
  OPhysicalMemoryManagement * pmm = OPhysicalMemoryManagement::instance();
  
  if (mbi->flags & MBI_MMAP)
  {
    // Cool, grub supplied mmap
    
    assert (mbi->mmap_addr < 0x40000000u);
    assert (mbi->mmap_addr + mbi->mmap_length < 0x40000000u);
    
    uint32_t offset = 0;
    while (offset < mbi->mmap_length)
    {
      struct multiboot_mmap_t * mmap = PTOKV(mbi->mmap_addr + offset, struct multiboot_mmap_t *);
      kpa_t base = mmap->base_addr_hi;
      base <<= 32;
      base |= mmap->base_addr_lo;
      
      kpa_t len = mmap->length_hi;
      len <<= 32;
      len |= mmap->length_lo;
      
      pmm->allocRegion(base, base + len, mmapToClass(mmap->type), mmapToUse(mmap->type),
		       pmm->describeClass(mmapToClass(mmap->type)));
      
      offset += mmap->size + 4; // size field is size of following bytes
    }
  } else if (mbi->flags & MBI_MEMVALID)
  {
    pmm->allocRegion(0, mbi->mem_lower * 1024, pmm->cls_RAM, pmm->use_Available,
		     pmm->describeClass(pmm->cls_RAM));
    pmm->allocRegion(0x00100000, mbi->mem_upper * 1024, pmm->cls_RAM, pmm->use_Available,
		     pmm->describeClass(pmm->cls_RAM));
  } else
  {
    // This braces are here because KDevelop keeps indenting the code below this without them... Damn...
    fatal ("Multiboot did not provide memory config!\n");
  }
  
  // Reserve some memory regions so that they do not get grabbed by the heap backing
  // storage allocator prematurely
  
  // Reserve some additional regions:
  pmm->allocRegion(0, 1024, pmm->cls_RAM, pmm->use_BIOS, "BIOS area");
  
#if MAX_NCPU > 1
  pmm->allocRegion(4096, 8192, pmm->cls_RAM, pmm->use_SMP, "SMP AP boot");
#endif
  
  /*
   * We need to guard the EBDA region. Not all BIOS memory queries report this region,
   * with the consequence that multiboot itself does not always identify it.
   * 
   * If multiboot did define it, it will have shown up as "reserved" and therefore will
   * have been reported as a ROM BIOS region. We will actually treat it as ROM, even though
   * it is a RAM region.
   * 
   * 16bit segment base of EBDA appears at the address 0x40e. First byte of the EBDA gives
   * size of EBDA in kilobytes.
   */
  uint32_t ebda_base = (* PTOKV(0x40e, uint16_t *)) << 4;
  uint32_t ebda_size = (* PTOKV(ebda_base, uint8_t *));
  ebda_size *= 1024;
  uint32_t ebda_bound = ebda_base + ebda_size;
  
  while (ebda_base < ebda_bound)
  {
    OPhysicalMemoryManagement::info_t * pmi = pmm->findRegion(ebda_base);
    uint32_t bound = ebda_bound;
    if (pmi->bound < ebda_bound)
      bound = pmi->bound;
    
    pmm->allocRegion(ebda_base, bound, pmm->cls_RAM, pmm->use_BIOS, "Extended BIOS Data Area");
    
    ebda_base = bound;
  }
}

/**
 * @brief Protect memory owned by the kernel
 */
void protectKernelMemory(void)
{
  OPhysicalMemoryManagement * pmm = OPhysicalMemoryManagement::instance();
  
  pmm->allocRegion(KVTOP(_start), KVTOP(_etext), pmm->cls_RAM, pmm->use_Kernel, "Kernel code");
  pmm->allocRegion(KVTOP(_rodata), KVTOP(_erodata), pmm->cls_RAM, pmm->use_Kernel, "Kernel rodata");
  pmm->allocRegion(KVTOP(__syscall_page), KVTOP(__esyscall_page),
		   pmm->cls_RAM, pmm->use_Kernel, "Syscall page");
  pmm->allocRegion(KVTOP(__begin_maps), KVTOP(__end_maps),
		   pmm->cls_RAM, pmm->use_Kernel, "Kernel mappings pages");
  pmm->allocRegion(KVTOP(cpu0_kstack_lo), KVTOP(cpu0_kstack_hi),
		   pmm->cls_RAM, pmm->use_Kernel, "CPU[0] stack");
  if (_pagedata != _epagedata)  // there is some page-aligned data
    pmm->allocRegion(KVTOP(_pagedata), KVTOP(_epagedata),
		     pmm->cls_RAM, pmm->use_Kernel, "Kernel page-aligned data");
  pmm->allocRegion(KVTOP(_data), KVTOP(_end), pmm->cls_RAM, pmm->use_Kernel, "Kernel data, bss & ctors");
}

/**
 * @brief Protect multiboot modules
 */
void protectMultibootModules(void)
{
  // First find out the physical locations of modules and command line.
  struct multiboot_info_t * mbi = PTOKV(multibootInfo, struct multiboot_info_t *);
  
  if (mbi->flags & MBI_CMDLINE) // Copy command line if available
    OCommandLine::instance()->set(mbi->cmdline);
  
/*  if (mbi->flags & MBI_MODS) // Get the modules if available
  {
    size_t nMods = mbi->mods_count;
    const struct multiboot_module_info_t * mmi = PTOKV(mbi->mods_addr, const struct multiboot_module_info_t *);
    
//     // First protect the existing modules
//     for (size_t i = 0; i < nMods; i++, mmi++)
//     {
//       const char * cmdLine = PTOKV(mmi->string, const char *);
//       
//       if (
  }*/
}

extern "C" void arch_init(void);

/**
 * @brief Perform architecture-specific initialization
 * 
 * arch_init() performs the low level initialization of the system before the real kernel starts
 * to work. It does the following tasks:
 * 
 * <ul>
 * <li>Initialize the boot console system that is used to display early bootup information. When the
 * kernel ends booting, the console will be shut down and released for use by user processes.</li>
 * <li>Initialize the physical memory so that memory ranges could be allocated.</li>
 * <li>Reserve memory space for kernel and page tables</li>
 * <li>Setup GDT, LDT, TSS and IDT tables.</li>
 * </ul>
 */
#include <typeinfo>
void arch_init(void)
{
  /* Initialize the console so that we can use printf for diagnostics.
   */
  OVgaConsole * vga = OVgaConsole::instance();
  vga->initialize();
  OConsole::stdout = vga;
  OConsole::stderr = vga;
  
  typeid(vga);
  
  /* Print Hello world :)
   */
  printf(ANSI_FG_WHITE "OOMTK version %s%s%s\n" ANSI_NORMAL,
	 ANSI_FG_CYAN, OOMTK::versionString(), ANSI_FG_WHITE);
  
  printf ("Initializing: ");
  
  /* Initialize the CPU structures
   */
  for (int ndx = 0; ndx < MAX_NCPU; ndx++)
  {
    OCPU::initialize(ndx); printf("CPU[%d], ", ndx);
  }
  
  /* Intitialize GDT & TSS tables
   */
  OGDT::instance()->initialize(); printf("GDT, ");
  OTSS::instance()->initialize(); printf("TSS, ");
  
  /* Initialize transient mappings
   */
  OPaging::instance()->transientMappingsInitialize(); printf("TransMap, ");
  
  /* Configure memory (get info on available physical memory and and protect memory regions)
   */
  OPhysicalMemoryManagement * pmm = OPhysicalMemoryManagement::instance();
  pmm->initialize(0, UsingPAE ? PAE_PADDR_BOUND : PTE_PADDR_BOUND);
  configurePhysicalMemory();
  protectKernelMemory();
  printf("PMM, ");
  
  // Findout how many regions are currently available before protecting multiboot regions
  // because they are going to be released afterwards...
  size_t totalPages = pmm->available(pmm->needPages, OOMTK_PAGE_SIZE, false);
  
  protectMultibootModules();
  
  OCommandLine::instance()->processOptions();
  
  OCPU::probeCPUs();
  
  for (;;);
}