#ifndef __ASM_MMU_H__
#define __ASM_MMU_H__
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/*
 * $Id: mmu.h 196 2006-10-23 13:29:52Z brcha $
 */
/** @file
 * @brief Assembler MMU definitions
 */

/*
 * An address:
 *  Page.dir.index (10bit), Page.table.index (10bit), Offset.in.page (12bit)
 */

/// @brief Page directory index
#define PDX(va)   ((((u32_t)(va)) >> 22) & 0x03FF)

/// @brief Page table index
#define PTX(va)   ((((u32_t)(va)) >> 12) & 0x03FF)

/// @brief Page number field of address
#define PPN(va)   (((u32_t)(va)) >> 12)
#define VPN(va)   PPN(va)

/// @brief Offset in page
#define PGOFF(va) (((u32_t)(va)) & 0xFFF)

// Conversions
/// @brief Page directory entries to a page directory
#define PDE2PD    1024
/// @brief Page table entries to a page table
#define PTE2PT    1024
/// @brief Bytes to a page
#define BY2PG     4096
/// @brief log2(BY2PG)
#define PGSHIFT   12
/// @brief Bytes mapped by a page dir. entry
#define PDMAP     (4*1024*1024)
/// @brief log2(PDMAP)
#define PDSHIFT   22

/**
 * Memory hole from 640k to 1M
 */
#define IOPHYSMEM   0xa0000
#define EXTPHYSMEM  0x100000

/**
 * Page Table/Directory Entry flags
 *  (see Intel manuals for more info)
 */
/// @brief Present
#define PTE_P     0x001
/// @brief Writable
#define PTE_W     0x002
/// @brief User
#define PTE_U     0x004
/// @brief Write-through
#define PTE_PWT   0x008
/// @brief Cache-disable
#define PTE_PCD   0x010
/// @brief Accessed
#define PTE_A     0x020
/// @brief Dirty
#define PTE_D     0x040
/// @brief Page size
#define PTE_PS    0x080
/// @brief Bits must be zero
#define PTE_MBZ   0x180
/// @brief Available for software use
#define PTE_AVAIL 0xe000
/// @brief All flags that can be used in system calls
#define PTE_USER  0xe07

/// @brief Address in PTE
#define PTE_ADDR(pte)   ((u32_t)(pte)&~0xfff)

/**
 * @defgroup CR
 * @brief Control register flags
 * @{
 */
/// @brief Protection Enable
#define CR0_PE    0x1
/// @brief Monitor coProcessor
#define CR0_MP    0x2
/// @brief Emulation
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

/// @brief Performance Counter Enable
#define CR4_PCE   0x100
/// @brief Machine Check Enable
#define CR4_MCE   0x40
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

/**
 * @defgroup EFlags
 * @brief EFlags register
 */
/// @brief Carry Flag
#define FL_CF     0x1
/// @brief Parity Flag
#define FL_PF     0x4
/// @brief Auxiliary carry Flag
#define FL_AF     0x10
/// @brief Zero Flag
#define FL_ZF     0x40
/// @brief Sign Flag
#define FL_SF     0x80
/// @brief Trap Flag
#define FL_TF     0x100
/// @brief Interrupt Flag
#define FL_IF     0x200
/// @brief Direction Flag
#define FL_DF     0x400
/// @brief Overflow Flag
#define FL_OF     0x800
/// @brief I/O Privilege Level (low bit)
#define FL_IOPL0  0x1000
/// @brief I/O Privilege Level (high bit)
#define FL_IOPL1  0x2000
/// @brief Nested Task
#define FL_NT     0x4000
/// @brief Resume Flag
#define FL_RF     0x10000
/// @brief Virtual 8086 mode
#define FL_VM     0x20000
/// @brief Alignment Check
#define FL_AC     0x40000
/// @brief Virtual Interrupt Flag
#define FL_VIF    0x80000
/// @brief Virtual Interrupt Pending
#define FL_VIP    0x100000
/// @brief ID flag
#define FL_ID     0x200000

/** @} */

/**
 * @defgroup PageFault
 * @brief Page fault error codes
 * @{
 */
/// @brief Page fault caused by protection violation
#define FEC_PR    0x1
/// @brief Page fault caused by a write
#define FEC_WR    0x2
/// @brief Page fault occured while in user mode
#define FEC_U     0x4

/**
 * @defgroup AppSegType
 * @brief Application segment type bits
 * @{
 */
/// @brief Executable segment
#define STA_X   0x8
/// @brief Expand down (no-exec)
#define STA_E   0x4
/// @brief Conforming code segment (exec-only)
#define STA_C   0x4
/// @brief Writeable (no-exec)
#define STA_W   0x2
/// @brief Readable (exec)
#define STA_R   0x2
/// @brief Accessed
#define STA_A   0x1

/** @} */

/**
 * @defgroup SysSegType
 * System segment type bits
 */
/// @brief Available 16bit TSS
#define STS_T16A    0x1
/// @brief Local Descriptor Table
#define STS_LDT     0x2
/// @brief Busy 16bit TSS
#define STS_T16B    0x3
/// @brief 16bit Call Gate
#define STS_CG16    0x4
/// @brief Task Gate
#define STS_TG      0x5
/// @brief 16bit Interrupt Gate
#define STS_IG16    0x6
/// @brief 16bit Trap Gate
#define STS_TG16    0x7
/// @brief Available 32bit TSS
#define STS_T32A    0x9
/// @brief Busy 32bit TSS
#define STS_T32B    0xb
/// @brief 32bit Call Gate
#define STS_CG32    0xc
/// @brief 32bit Interrupt Gate
#define STS_IG32    0xe
/// @brief 32bit Trap Gate
#define STS_TG32    0xf

#ifdef __ASSEMBLER__

/**
 * @defgroup GDTentries
 * @brief Macros to build GDT entries in assembly
 * @{
 */
/// @brief Null segment
#define SEG_NULL            \
          .word 0, 0;       \
          .byte 0, 0, 0, 0
/// @brief Normal segment
#define SEG(type,base,lim)                                      \
          .word (((lim)>>12)&0xffff), ((base)&0xffff);          \
          .byte (((base)>>16)&0xff), (0x90|(type)),             \
                (0xc0|(((lim)>>28)&0xf)), (((base)>>24)&0xff)
/** @} */

#endif // __ASSEMBLER__

#endif /* __ASM_MMU_H__ */
