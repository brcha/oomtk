#ifndef __ASM_PMAP_H__
#define __ASM_PMAP_H__
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/*
 * $Id: pmap.h 196 2006-10-23 13:29:52Z brcha $
 */
/** @file
 * @brief Definitions for memory management
 */

#include INC_ARCH(mmu.h)

/**
 * @defgroup GDTn
 * @brief Global Descriptor Tables' numbers
 * @{
 */
/// @brief Kernel Code
#define GDT_KC      0x08
/// @brief Kernel Data
#define GDT_KD      0x10
/// @brief User Code
#define GDT_UC      0x18
/// @brief User Data
#define GDT_UD      0x20
/// @brief Task segment selector
#define GDT_TSS     0x28

/** @} */

/// @brief Start of the Kernel virtual space
#define KERNEL_BASE   0xf0000000

/**
 * Virtual memory map:
 *
 * From KERNEL_BASE up to 4 GB => only kernel can access, that is kernel space (ring 0)
 *
 * From KERNEL_BASE-PDMAP to KERNEL_BASE is a Virtual Page Table
 *
 * From VPT - PDMAP to VPT is Kernel Stack
 *
 * From 0 to USER_LIMIT is Userspace (ring 3)
 *
 * From USER_LIMIT - PDMAP to USER_LIMIT is User VPT (USER_VPT)
 *
 * From USER_VPT - PDMAP to USER_VPT is User pages (USER_PAGES)
 *
 * From USER_PAGES - PDMAP to USER_PAGES is User environments (R/O)
 *
 * From USER_ENVS - BY2PG to USER_ENVS is user exception stack (UXSTACKTOP = USER_ENVS)
 *
 * At USER_ENVS - 2*BY2PG is User Stack TOP
 *
 * From 0 to UCODE is User code space (size = 2*PDMAP)
 */

/**
 * @brief Virtual page table
 * Entry PDX[VPT] in PD has a pointer to PD => PD is a page table which maps all
 * PTEs for virtual address space into 4 MB region starting at VPT
 */
#define VPT (KERNEL_BASE - PDMAP)
#define KSTACKTOP   VPT
#define KSTACKSIZE  (8*BY2PG)
#define USER_LIMIT  (KSTACKTOP - PDMAP)

/**
 * User read-only mappings of the structures
 */

/// @brief User ro copy of VPT
#define USER_VPT    (USER_LIMIT - PDMAP)
/// @brief Page structures (ro copy)
#define USER_PAGES  (USER_VPT - PDMAP)
/// @brief Global environment structures (ro copy)
#define USER_ENVS   (USER_PAGES - PDMAP)

/**
 * User space
 */
#define USER_TOP          USER_ENVS
/// @brief Page exception stack
#define UXSTACKTOP   (USER_TOP)
/// @brief Normal user stack
#define USTACKTOP    (USER_TOP - 2*BY2PG)
#define USER_CODE    (2*PDMAP)

/**
 * Virtual 2 physical address
 */
#define VA(x)       ((x)-KERNEL_BASE)
/**
 * Physical 2 virtual address
 */
#define PA(x)       ((x)+KERNEL_BASE)

#endif /* __ASM_PMAP_H__ */
