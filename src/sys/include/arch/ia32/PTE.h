#ifndef __IA32_PTE_H__
#define __IA32_PTE_H__
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** @file
 * @brief Page Table flags
 */

/**
 * @defgroup PTE
 * @brief Page Table/Directory Entry flags
 *  (see Intel manuals for more info)
 * @{
 */
/// @brief Present (also known as Valid)
#define PTE_P     0x001
#define PTE_V     PTE_P
/// @brief Writable (R/W)
#define PTE_W     0x002
/// @brief User (User/Supervisor)
#define PTE_USER  0x004
/// @brief Write-through
#define PTE_PWT   0x008
/// @brief Cache-disable
#define PTE_PCD   0x010
/// @brief Accessed
#define PTE_A     0x020
#define PTE_ACC   PTE_A
/// @brief Dirty
#define PTE_D     0x040
#define PTE_DRTY  PTE_D
/// @brief Page size (PDE, Pentium+ only)
#define PTE_PS    0x080
/// @brief Global page (PDE, PTE, Pentium Pro+ only)
#define PTE_GLBL  0x100
/// @brief Available for software use
#define PTE_SW0   0x200
/// @brief Available for software use
#define PTE_SW1   0x400
/// @brief Available for software use
#define PTE_SW2   0x800

/// @brief Mask to get PTE Base Address
#define PTE_FRAMEBITS   0xfffff000
/// @brief Mask to get info bits (permissions & stuff)
#define PTE_INFOBITS    0x00000fff
/** @} */

/**
 * @defgroup PAE
 * @brief Physical Address Extension
 * @{
 */
/// @brief Present (or valid)
#define PAE_P     0x001
#define PAE_V     PAE_P
/// @brief Writable
#define PAE_W     0x002
/// @brief User
#define PAE_USER  0x004
/// @brief Write-through
#define PAE_PWT   0x008
/// @brief Cache-disable
#define PAE_PCD   0x010
/// @brief Accessed
#define PAE_A     0x020
#define PAE_ACC   PAE_A
/// @brief Dirty
#define PAE_D     0x040
#define PAE_DRTY  PAE_D
/// @brief Page Size
#define PAE_PS    0x080
/// @brief Global page
#define PAE_GLBL  0x100
/// @brief Available for software use
#define PAE_SW0   0x200
/// @brief Available for software use
#define PAE_SW1   0x400
/// @brief Available for software use
#define PAE_SW2   0x800

#endif /* __IA32_PTE_H__ */
