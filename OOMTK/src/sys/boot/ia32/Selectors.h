/* -*-C++-*- */
/*
 *  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file Selectors.h
 * \brief Segment selector definitions
 */
/*
 * $Id: $
 */
#ifndef __SELECTORS_H__
#define __SELECTORS_H__

/* NOTE: This header should be includable from the assembler */
/* These selectors used to be enum in the class GDT, but that can't be used in asm :) */

/// \brief Null segment
#define seg_NULL      0x0
/// \brief Kernel's code segment
#define seg_KernelCS  0x1
/// \brief Kernel's data segment
#define seg_KernelDS  0x2
/// \brief User's code segment
#define seg_UserCS    0x3
/// \brief User's data segment
#define seg_UserDS    0x4
/// \brief User's thread-local storage
#define seg_UserTLS   0x5
/// \brief Task-switch segment
#define seg_TSS       0x6
/// \brief Number of segments
#define seg_Count     0x7

/**
 * \brief Get the segment selector from the segment index
 * \param seg segment index
 * \param rpl privilege level
 * \returns segment selector
 */
#define SELECTOR(seg,rpl) (((seg) << 3) + (rpl))

/// \brief Null segment selector
#define sel_NULL       SELECTOR(seg_NULL, 0)
/// \brief Kernel code segment selector
#define sel_KernelCS   SELECTOR(seg_KernelCS, 0)
/// \brief Kernel data segment selector
#define sel_KernelDS   SELECTOR(seg_KernelDS, 0)
/// \brief User code segment selector
#define sel_UserCS     SELECTOR(seg_UserCS, 3)
/// \brief User data segment selector
#define sel_UserDS     SELECTOR(seg_UserDS, 3)
/// \brief User thread-local storage selector
#define sel_UserTLS    SELECTOR(seg_UserTLS, 3)
/// \brief TSS segment selector
#define sel_TSS        SELECTOR(seg_TSS, 0)

#endif /* __SELECTORS_H__ */
