INTERFACE:
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** @file
 * @brief Page tables & paging
 *
 * For more information about page tables take a look at the Intel's IA-32 manuals
 */

#include <types.h>

/**
 * @brief Page Directory Entry
 *
 * NOTE: On Pentium II or earlier Page Size must be 0
 */
union PageDirectoryEntry {
  struct {
    u32_t present   : 1;  ///< @brief Present
    u32_t rw        : 1;  ///< @brief Read/Write
    u32_t userSuper : 1;  ///< @brief User/Supervisor
    u32_t pwt       : 1;  ///< @brief Page Write-Through
    u32_t pcd       : 1;  ///< @brief Page Cache Disabled
    u32_t accessed  : 1;  ///< @brief Accessed
    u32_t s         : 1;  ///< @brief Reserved (set to 0)
    u32_t ps        : 1;  ///< @brief Page size (0 indicates 4 KB)
    u32_t global    : 1;  ///< @brief Global page (ignored)
    u32_t avail     : 3;  ///< @brief Available for system programmer's use
    u32_t pt_base   :20;  ///< @brief Page-Table Base Address
  } bits4k;               ///< @brief Page directory entry bits for 4 KB pages
  struct {
    u32_t present   : 1;  ///< @brief Present
    u32_t rw        : 1;  ///< @brief Read/Write
    u32_t userSuper : 1;  ///< @brief User/Supervisor
    u32_t pwt       : 1;  ///< @brief Page Write-Through
    u32_t pcd       : 1;  ///< @brief Page Cache Disabled
    u32_t accessed  : 1;  ///< @brief Accessed
    u32_t dirty     : 1;  ///< @brief Dirty
    u32_t ps        : 1;  ///< @brief Page size (1 indicates 4 MB)
    u32_t global    : 1;  ///< @brief Global page
    u32_t avail     : 3;  ///< @brief Available for system programmer's use
    u32_t pat       : 1;  ///< @brief Page Table Attribute Index (set to 0 if not supported)
    u32_t reserved  : 9;  ///< @brief Reserved (must be set to 0)
    u32_t page_base :10;  ///< @brief Page Base Address
  } bits4m;               ///< @brief Page directory entry bits for 4 MB pages
  u32_t value;            ///< @brief Page directory entry 32bit value
};

/**
 * @brief Page Table Entry
 */
union PageTableEntry {
  struct {
    u32_t present   : 1;  ///< @brief Present
    u32_t rw        : 1;  ///< @brief Read/Write
    u32_t userSuper : 1;  ///< @brief User/Supervisor
    u32_t pwt       : 1;  ///< @brief Page Write-Through
    u32_t pcd       : 1;  ///< @brief Page Cache Disabled
    u32_t accessed  : 1;  ///< @brief Accessed
    u32_t dirty     : 1;  ///< @brief Dirty
    u32_t pat       : 1;  ///< @brief Page Table Attribute Index (set to 0 if not supported)
    u32_t global    : 1;  ///< @brief Global page
    u32_t avail     : 3;  ///< @brief Available for system programmer's use
    u32_t page_base :20;  ///< @brief Page Base Address
  } bits4k;               ///< @brief Page table entry bits for 4 KB pages
  u32_t value;            ///< @brief Page table entry 32bit value
};

IMPLEMENTATION: