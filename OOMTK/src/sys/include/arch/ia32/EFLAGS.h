#ifndef __IA32_EFLAGS_H__
#define __IA32_EFLAGS_H__
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** @file
 * @brief EFlags bits definitions
 */

/**
 * @defgroup EFlags
 * @brief EFlags register
 */
/// @brief Carry Flag
#define EFLAGS_CF     0x1
/// @brief Parity Flag
#define EFLAGS_PF     0x4
/// @brief Auxiliary carry Flag
#define EFLAGS_AF     0x10
/// @brief Zero Flag
#define EFLAGS_ZF     0x40
/// @brief Sign Flag
#define EFLAGS_SF     0x80
/// @brief Trap Flag
#define EFLAGS_TF     0x100
/// @brief Interrupt Flag
#define EFLAGS_IF     0x200
/// @brief Direction Flag
#define EFLAGS_DF     0x400
/// @brief Overflow Flag
#define EFLAGS_OF     0x800
/// @brief I/O Privilege Level (mask to get out of flags)
#define EFLAGS_IOPL   0x3000

// These are for comparing with the EFLAGS_IOPL masked %eflags
/// @brief I/O Privilege Level 0
#define EFLAGS_IOPL0  0x0000
/// @brief I/O Privilege Level 1
#define EFLAGS_IOPL1  0x1000
/// @brief I/O Privilege Level 2
#define EFLAGS_IOPL2  0x2000
/// @brief I/O Privilege Level 3
#define EFLAGS_IOPL3  0x3000

/// @brief Nested Task
#define EFLAGS_NT     0x4000
/// @brief Resume Flag
#define EFLAGS_RF     0x10000
/// @brief Virtual 8086 mode
#define EFLAGS_VM     0x20000
/// @brief Alignment Check
#define EFLAGS_AC     0x40000
/// @brief Virtual Interrupt Flag
#define EFLAGS_VIF    0x80000
/// @brief Virtual Interrupt Pending
#define EFLAGS_VIP    0x100000
/// @brief ID flag
#define EFLAGS_ID     0x200000
/** @} */

#endif /* __IA32_EFLAGS_H__ */
