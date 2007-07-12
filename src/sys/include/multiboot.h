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
 * @brief Multiboot classes
 */

#include <types.h>
#include <stddef.h>

/**
 * Multiboot header
 */
class KMultibootHeader
{
public:
  /** Some flags from multiboot standard */
  enum {
    /**
     * The entire multiboot header must be contained within the first
     * MULTIBOOT_SEARCH bytes of the kernel image
     */
    Search = 8192,

    /**
     * Magic value identifying the multiboot header
     */
    Magic = 0x1badb002,

    /**
     * Features flags for 'flags'.
     * If a boot loader sees a flag in MULTIBOOT_MUSTKNOW set and
     * it doesn't understand it, it must fail.
     */
    Mustknow = 0x0000ffff,

    /**
     * Align all boot modules on page (4KB) boundaries
     */
    PageAlign = 0x00000001,

    /**
     * Must be provided memory information in multiboot info structure
     */
    MemoryInfo = 0x00000002,

    /**
     * Use the load address fields above instead of the ones in the a.out
     * header to figure out what to load where, and what to do afterwards.
     * This should be needed only for a.out kernel imaged (ELF and other
     * formats can generally provide the needed information.
     */
    AoutKludge = 0x00010000,

    /**
     * The boot loader passes this value in register EAX to signal the
     * kernel that the multiboot method is being used
     */
    Valid = 0x2badb002,
  };

  /** MULTIBOOT_MAGIC */
  u32_t	magic;

  /** Feature flags */
  u32_t flags;

  /** Checksum: magic + flags + checksum == 0 */
  u32_t checksum;

  /* These are valid for a.out kludge */
  addr_t headerAddr;
  addr_t loadAddr;
  addr_t loadEndAddr;
  addr_t bssEndAddr;
  addr_t entry;
};

/**
 * Multiboot VBE controller information
 */
class KMultibootVBEController
{
public:
  u8_t	signature[4];
  u16_t	version				__attribute__((packed));
  u32_t	oemString			__attribute__((packed));
  u32_t	capabilities			__attribute__((packed));
  u32_t	videoMode			__attribute__((packed));
  u16_t	totalMemory			__attribute__((packed));
  u16_t	oemSoftwareRev			__attribute__((packed));
  u32_t	oemVendorName			__attribute__((packed));
  u32_t	oemProductName			__attribute__((packed));
  u32_t	oemProductRev			__attribute__((packed));
  u8_t	reserved[222]			__attribute__((packed));
  u8_t	oemData[256]			__attribute__((packed));
};

/**
 * Multiboot VBE mode information
 */
class KMultibootVBEMode
{
public:
  u16_t	modeAttributes;
  u8_t	winAAttributes			__attribute__((packed));
  u8_t	winBAttributes			__attribute__((packed));
  u16_t	winGranularity			__attribute__((packed));
  u16_t	winSize				__attribute__((packed));
  u16_t	winASegment			__attribute__((packed));
  u16_t	winBSegment			__attribute__((packed));
  u32_t	winFunction			__attribute__((packed));
  u16_t	bytesPerScanline		__attribute__((packed));

  /* >= 1.2 */
  u16_t	xResolution			__attribute__((packed));
  u16_t	yResolution			__attribute__((packed));
  u8_t	xCharSize			__attribute__((packed));
  u8_t	yCharSize			__attribute__((packed));
  u8_t	numberOfPlanes			__attribute__((packed));
  u8_t	bitsPerPixel			__attribute__((packed));
  u8_t	numberOfBanks			__attribute__((packed));
  u8_t	memoryModel			__attribute__((packed));
  u8_t	bankSize			__attribute__((packed));
  u8_t	numberOfImagePages		__attribute__((packed));
  u8_t	reserved0			__attribute__((packed));
  
  /* direct color */
  u8_t	redMaskSize			__attribute__((packed));
  u8_t	redFieldPosition		__attribute__((packed));
  u8_t	greenMaskSize			__attribute__((packed));
  u8_t	greenFieldSize			__attribute__((packed));
  u8_t	blueMaskSize			__attribute__((packed));
  u8_t	blueFieldPosition		__attribute__((packed));
  u8_t	reservedMaskSize		__attribute__((packed));
  u8_t	reservedFieldPosition		__attribute__((packed));
  u8_t	directColorModeInfo		__attribute__((packed));

  /* >= 2.0 */
  u32_t	physBase			__attribute__((packed));
  u32_t	reserved1			__attribute__((packed));
  u16_t	reserved2			__attribute__((packed));

  /* >= 3.0 */
  u16_t	linearBytesPerScanline		__attribute__((packed));
  u8_t	bankedNumberOfImagePages	__attribute__((packed));
  u8_t	linearNumberOfImagePages	__attribute__((packed));
  u8_t	linearRedMaskSize		__attribute__((packed));
  u8_t	linearRedFieldPosition		__attribute__((packed));
  u8_t	linearGreenMaskSize		__attribute__((packed));
  u8_t	linearGreenFieldPosition	__attribute__((packed));
  u8_t	linearBlueMaskSize		__attribute__((packed));
  u8_t	linearBlueFieldPosition		__attribute__((packed));
  u8_t	linearReservedMaskSize		__attribute__((packed));
  u8_t	linearReservedFieldPosition	__attribute__((packed));
  u32_t	maxPixelClock			__attribute__((packed));
  
  u8_t	reserved3[189]			__attribute__((packed));
};

/**
 * Multiboot info
 */
class KMultibootInfo
{
public:
  enum {
    Memory	= (1L <<  0),
    BootDevice	= (1L <<  1),
    CmdLine	= (1L <<  2),
    Modules	= (1L <<  3),
    AoutSyms	= (1L <<  4),
    ElfShdr	= (1L <<  5),
    MemMap	= (1L <<  6),
    DriveInfo	= (1L <<  7),
    CfgTable	= (1L <<  8),
    BootLdName	= (1L <<  9),
    ApmTable	= (1L << 10),
    VideoInfo	= (1L << 11),
  };

  /**
   * These flags indicate which parts of the multiboot info are used.
   */
  u32_t	    flags;

  /**
   * Amount of lower memory installed in the machine.
   * Valid only if MULTIBOOT_MEMORY flag is set.
   */
  size_t    memLower;

  /**
   * Amound of upper memory installed in the machine.
   * Valid only if MULTIBOOT_MEMORY flag is set.
   */
  size_t    memUpper;

  /**
   * BIOS disk device the kernel was loaded from.
   * Valid only if MULTIBOOT_BOOT_DEVICE flag is set.
   */
  u8_t	    bootDevice[4];

  /**
   * Command line for the OS kernel
   * A NULL-terminated ASCII string.
   * Valid only if MULTIBOOT_CMDLINE flag is set.
   */
  addr_t    cmdLine;

  /**
   * List of boot modules loaded with the kernel.
   * Valid only if MULTIBOOT_MODS flag is set.
   */
  u32_t	    modulesCount;
  addr_t    modulesAddress;

  /**
   * Symbol information for a.out or ELF executables
   */
  union {
    struct {
      /* a.out symbol info valid only if MULTIBOOT_AOUT_SYMS
       * is set in the flags
       */
      size_t	tabSize;
      size_t	strSize;
      addr_t	addr;
      u32_t	reserved;
    } aOut;

    struct {
      /* ELF section header information valid only if
       * MULTIBOOT_ELF_SHDR flag is set
       */
      u32_t	num;
      size_t	size;
      addr_t	addr;
      u32_t	shndx;
    } elf;
  } syms;

  /**
   * Memory map buffer
   * Valid only if MULTIBOOT_MEM_MAP flag is set
   */
  size_t    mmapCount;
  addr_t    mmapAddress;

  /**
   * Drive info buffer
   */
  u32_t	    drivesLength;
  u32_t	    drivesAddress;

  /**
   * ROM Configuration Table
   */
  u32_t	    configTable;

  /**
   * Boot loader name
   */
  u32_t	    bootLoaderName;

  /**
   * APM table
   */
  u32_t	    apmTable;

  /**
   * Video
   */
  u32_t	    vbeControlInfo;
  u32_t	    vbeModeInfo;
  u16_t	    vbeMode;
  u16_t	    vbeInterfaceSegment;
  u16_t	    vbeInterfaceOffset;
  u16_t	    vbeInterfaceLength;
};

/**
 * Multiboot module structure
 */
class KMultibootModule
{
  public:
    /**
     * Physical start address of the module data
     */
    addr_t  moduleStart;
    /**
     * Physical end address of the module data
     */
    addr_t  moduleEnd;

    /**
     * Arbitrary ASCII string associated with the module
     */
    addr_t  string;

    /**
     * Boot loader must set to 0; OS should ignore this.
     */
    u32_t   reserved;
};

/**
 * The memory map.
 *
 * The mmap_addr field contains the physical address of the first of the
 * memory map structures. "size" represents the size of the rest of the
 * structure and optional padding. The offset to the beginning of the
 * next structure is therefore "size+4"
 */
struct MB_MemoryMap
{
  u32_t	size;
  u32_t	baseAddrLow;
  u32_t	baseAddrHigh;
  u32_t	lengthLow;
  u32_t	lengthHigh;
  u32_t	type;
  /* unspecified optional padding ... */
};

#endif /* __MULTIBOOT_H__ */
