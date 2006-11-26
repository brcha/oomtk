#ifndef __IA32_CR_H__
#define __IA32_CR_H__
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** @file
 * @brief Definitions of IA32 CR? register bits
 *
 * CR0 - the main control register
 * CR1 - reserved
 * CR2 - page fault linear address (usefull for malloc)
 * CR3 - page directory base register
 */

/**
 * @defgroup CR0
 * @brief Control register flags
 *
 * Normal state [PG=1, CD=0, NW=0, AM=1, WP=1, NE=1, ET=1, TS=1, EM=0, MP=1, PE=1]
 *
 * @{
 */
/// @brief Protection Enable
#define CR0_PE    0x1
/// @brief Monitor coProcessor
#define CR0_MP    0x2
/// @brief numeric EMulation
#define CR0_EM    0x4
/// @brief Task Switched
#define CR0_TS    0x8
/// @brief Extension Type
#define CR0_ET    0x10
/// @brief Numeric Error
#define CR0_NE    0x20
/// @brief Write Protect
#define CR0_WP    0x10000
/// @brief Alignment Mask
#define CR0_AM    0x40000
/// @brief Not Writethrough
#define CR0_NW    0x20000000
/// @brief Cache Disable
#define CR0_CD    0x40000000
/// @brief PaGing
#define CR0_PG    0x80000000
/** @} */

/**
 * @defgroup CR3
 * @brief Page directory control register
 * @{
 */
/// @brief Page-level write transparent
#define CR3_PWT   0x4
/// @brief Page-level cache disable
#define CR3_PCD   0x8
/** @} */

/**
 * @defgroup CR4
 * @brief Additional control register flags
 *
 * Normal state [VME=0, PVI=0, TSD=0, DE=1, PSE=1, PAE=1, MCE=0, PGE=1, PCE=1,
 *               OSFXSR=0, OSXMMEXCEPT=0]
 *
 * When support is added for other stuff (such as Machine Check or OS*), it will
 * be enabled.
 *
 * I don't see any use for VM8086, so I will not enable it and will not add support
 * for that unless someone makes me think that such a thing is very important for
 * survival, or something...
 *
 * Debuging extensions are enabled and I don't see why they would not be enabled.
 * Again, if somebody thinks it should be disabled, that somebody should advise
 * me to do so.
 *
 * @{
 */
/// @brief OS supports SIMD FP exceptions
#define CR4_OSXMMEXCEPT 0x400
/// @brief OS supports FXSAVE & FSTSTOR
#define CR4_OSFXSR  0x200
/// @brief Performance Counter Enable
#define CR4_PCE   0x100
/// @brief Page Global Enable
#define CR4_PGE   0x80
/// @brief Machine Check Enable
#define CR4_MCE   0x40
/// @brief Physical Address Extension
#define CR4_PAE   0x20
/// @brief Page Size Extensions
#define CR4_PSE   0x10
/// @brief Debugging Extensions
#define CR4_DE    0x08
/// @brief Time-Stamp Disable
#define CR4_TSD   0x04
/// @brief Protected-mode Virtual Interrupts
#define CR4_PVI   0x02
/// @brief V86 Mode Extensions
#define CR4_VME   0x01
/** @} */

#endif /* __IA32_CR_H__ */
