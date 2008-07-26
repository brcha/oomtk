/* -*-C++-*- */
/*
 *  Copyright (C) 2007 by Filip Brcic <brcha@gna.org>
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
/** @file Selectors.h
 * @brief Segment selector definitions
 */
#ifndef __IA32_SELECTORS_H__
#define __IA32_SELECTORS_H__

/* NOTE: This header should be includable from the assembler */
/* These selectors used to be enum in the class GDT, but that can't be used in asm :) */

/// @brief Null segment
#define seg_NULL        0x0
/// @brief Task-switch segment
#define seg_TSS         0x1
/// @brief Kernel's code segment
#define seg_KernelCode  0x2
/// @brief Kernel's data segment
#define seg_KernelData  0x3
/// @brief User's code segment
#define seg_UserCode    0x4
/// @brief User's data segment
#define seg_UserData    0x5
/// @brief User's thread-local storage
#define seg_UserTLS     0x6
/// @brief Number of segments
#define seg_Count       0x7

/// @brief Kernel's privilege level
#define pl_Kernel       0x0
/// @brief User's privilege level
#define pl_User         0x3

/**
 * @brief Get the segment selector from the segment index
 * @param seg segment index
 * @param rpl privilege level
 * @returns segment selector
 */
#define SELECTOR(seg,rpl) (((seg) << 3) + (rpl))

/// @brief Null segment selector
#define sel_NULL        SELECTOR(seg_NULL, pl_Kernel)
/// @brief TSS segment selector
#define sel_TSS         SELECTOR(seg_TSS, pl_Kernel)
/// @brief Kernel code segment selector
#define sel_KernelCode  SELECTOR(seg_KernelCode, pl_Kernel)
/// @brief Kernel data segment selector
#define sel_KernelData  SELECTOR(seg_KernelData, pl_Kernel)
/// @brief User code segment selector
#define sel_UserCode    SELECTOR(seg_UserCode, pl_User)
/// @brief User data segment selector
#define sel_UserData    SELECTOR(seg_UserData, pl_User)
/// @brief User thread-local storage selector
#define sel_UserTLS     SELECTOR(seg_UserTLS, pl_User)

#endif /* __IA32_SELECTORS_H__ */
