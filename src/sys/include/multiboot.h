#ifndef __MULTIBOOT_H__
#define __MULTIBOOT_H__
/*
 *  Copyright (C) 1994-2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK (http://launchpad.net/oomtk)
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
/** @file
 * @brief Multiboot structures
 */

/** @brief Multiboot header magic
 */
#define MULTIBOOT_HEADER_MAGIC		0x1BADB002

/* Multiboot header flags */
#define MBH_FL_PageAlign		0x1	/**< @brief Align modules to page (4k) boundary */
#define MBH_FL_MemInfo			0x2	/**< @brief Require memory info fields		*/
#define MBH_FL_VideoMode		0x4	/**< @brief Require video modes table		*/

/** @brief Multiboot header flags
 * 
 * Flags = PageAlign | MemInfo | VideoMode
 */
#define MULTIBOOT_HEADER_FLAGS		0x7

/** @brief Multiboot bootloader magic
 */
#define MULTIBOOT_BOOTLOADER_MAGIC	0x2BADB002

/* Now the C part */
#ifndef __ASSEMBLER__

#include <types.h>
#include <stddef.h>

/** @brief Multiboot header structure
 */
struct multiboot_header_t
{
  /** @brief multiboot magic */
  uint32_t	magic;

  /** @brief multiboot feature flags */
  uint32_t	flags;

  /** @brief Checksum: magic + flags + checksum == 0 */
  uint32_t	checksum;

  /* The following fields are valid for a.out kludge */
  uint32_t	header_addr;
  uint32_t	load_addr;
  uint32_t	load_end_addr;
  uint32_t	bss_end_addr;
  uint32_t	entry_addr;
  
  /* The following fields are valid if VideoMode flag is set */
  uint32_t	mode_type;
  uint32_t	width;
  uint32_t	height;
  uint32_t	depth;
};

/* Multiboot info flags */
#define MBI_MEMVALID  0x001	/**< @brief mem_* fields valid		*/
#define MBI_BOOTDEV   0x002	/**< @brief boot_device fields valid	*/
#define MBI_CMDLINE   0x004	/**< @brief cmdline valid		*/
#define MBI_MODS      0x008	/**< @brief mods valid			*/
#define MBI_SYMTAB    0x010	/**< @brief symtab valid		*/
#define MBI_SECHDR    0x020	/**< @brief section header valid	*/
#define MBI_MMAP      0x040	/**< @brief mmap_* fields valid		*/
#define MBI_DRIVES    0x080	/**< @brief drives_* fields valid	*/
#define MBI_CFGTBL    0x100	/**< @brief config table valid		*/
#define MBI_BOOTLDR   0x200	/**< @brief bootloader name valid	*/
#define MBI_APMTBL    0x400	/**< @brief APM table valid		*/
#define MBI_GFXTBL    0x800	/**< @brief graphics table valid	*/

/** @brief Multiboot info
 */
struct multiboot_info_t
{
  uint32_t  flags;      /**< @brief indicate which parts of the multiboot info are valid. */
  
  /* valid if bit 0 is set: */
  uint32_t  mem_lower;  /**< @brief in KB, from 0 to 640 */
  uint32_t  mem_upper;  /**< @brief in KB, from 1MB */
  
  /* valid if bit 1 is set: */
  struct {
    uint8_t drive;
    uint8_t part1;
    uint8_t part2;
    uint8_t part3;
  } boot_device;
  
  /* valid if bit 2 is set: */
  uint32_t  cmdline;    /**< @brief physicall addr of the cmdline string */
  
  /* valid if bit 3 is set: */
  uint32_t  mods_count; /**< @brief number of modules */
  uint32_t  mods_addr;  /**< @brief of modules struct */
  
  /* valid if bit 4 or bit 5 is set (mutually exclusive): */
  union {
    struct {
      uint32_t  tabsize;    /**< @brief number of nlist structs */
      uint32_t  strsize;
      uint32_t  addr;       /**< @brief of table of nlist structs */
      uint32_t  : 32;       /* nothing */
    } aout; /* if bit 4 is set */
    struct {
      uint32_t  num;        /**< @brief of elf sect. headers */
      uint32_t  size;       /**< @brief of each entry */
      uint32_t  addr;       /**< @brief pointer */
      uint32_t  shndx;      /**< @brief string table */
    } elf;  /* if bit 5 is set */
  } u;
  
  /* valid if bit 6 is set: */
  uint32_t  mmap_length;
  uint32_t  mmap_addr;
  
  /* valid if bit 7 is set: */
  uint32_t  drives_length;
  uint32_t  drives_addr;
  
  /* valid if bit 8 is set: */
  uint32_t  config_table;
  
  /* valid if bit 9 is set: */
  uint32_t  boot_loader_name;
  
  /* valid if bit a is set: */
  uint32_t  apm_table;
  
  /* valid if bit b is set: */
  uint32_t  vbe_control_info;   /**< @brief as returned by VBE fn 0x00 */
  uint32_t  vbe_mode_info;      /**< @brief as returned by VBE fn 0x01 */
  uint32_t  vbe_mode;           /**< @brief current mode */
  uint32_t  vbe_iface_segment;
  uint32_t  vbe_iface_offset;
  uint32_t  vbe_iface_length;
};

/** @brief Multiboot modules information
 */
struct multiboot_module_info_t
{
  uint32_t  mod_start;
  uint32_t  mod_end;
  uint32_t  string;
  uint32_t  : 32;       /* reserved */
};

/** @brief Multiboot mmap
 */
struct multiboot_mmap_t
{
  uint32_t  size;
  uint32_t  base_addr_lo;
  uint32_t  base_addr_hi;
  uint32_t  length_lo;
  uint32_t  length_hi;
  uint32_t  type;
};

/** @brief Multiboot drive info
 */
struct multiboot_drive_t
{
  uint32_t  size;             /**< @brief distance to next */
  uint8_t   drive_number;     /**< @brief BIOS drive no */
  uint8_t   drive_mode;       /**< @brief 0 = CHS, 1 = LBA */
  uint16_t  drive_cylinders;
  uint8_t   drive_heads;
  uint8_t   drive_sectors;
  uint16_t  drive_ports[0];   /**< @brief I/O ports used to talk with this drive. List ended with 0. */
};

/** @brief Multiboot APM
 */
struct multiboot_apm_t
{
  uint16_t  version;
  uint16_t  cseg;
  uint32_t  offset;
  uint16_t  cseg_16;
  uint16_t  dseg;
  uint16_t  flags;
  uint16_t  cseg_len;
  uint16_t  cseg_16_len;
  uint16_t  dseg_len;
};

#endif /* ! __ASSEMBLER__ */

#endif /* __MULTIBOOT_H__ */
