#ifndef __GDT_H__
#define __GDT_H__
/*
 *  Copyright (C) 2006-2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file GDT.h
 * \brief Global Descriptor Table
 */
/*
 * $Id$
 */

#include <types.h>

/**
 * \brief Global Descriptor Table - GDT
 */

class GDT
{
  public:
    /**
     * \brief Get one instance of this class
     * \returns the instance
     */

    static GDT * instance();

    /**
     * \brief Setup the Global Descriptor Table
     */
    void setup();

  protected:
    /**
     * \brief Protected constructor
     */
    GDT();

    /**
     * \brief Get the segment selector from the segment index
     * \param seg segment index
     * \param rpl privilege level
     * \returns segment selector
     */
    inline word_t selector(word_t seg, word_t rpl)
    {
      // selector = (index<<3) + (ti << 2) + rpl, but ti=0 since this is GDT
      return (seg << 3) + rpl;
    };

    /**
    * \returns NULL segment selector
     */
    inline word_t selNull()
    {
      return selector(seg_NULL, 0);
    };

    /**
     * \returns Kernel code segment selector
     */
    inline word_t selKernelCS()
    {
      return selector(seg_KernelCS, 0);
    };

    /**
     * \returns TSS segment selector
     */
    inline word_t selTSS()
    {
      return selector(seg_TSS, 0);
    };

    /**
     * \returns Kernel data segment selector
     */
    inline word_t selKernelDS()
    {
      return selector(seg_KernelDS, 0);
    };

    /**
     * \returns User's code segment selector
     */
    inline word_t selUserCS()
    {
      return selector(seg_UserCS, 3);
    };

    /**
     * \returns User's data segment selector
     */
    inline word_t selUserDS()
    {
      return selector(seg_UserDS, 3);
    };

    /**
     * \returns User's thread-local storage segment selector
     */
    inline word_t selUserTLS()
    {
      return selector(seg_UserTLS, 3);
    };

    /**
     * \brief Setup the segment limit and base
     * \param seg segment index
     */
    inline void setupLimitBase(word_t seg, uint32_t limit, uint32_t base)
    {
      // Setup the limit
      gdt[seg].bits.limit_lo  = limit & 0x0000ffff;
      gdt[seg].bits.limit_hi  = (limit & 0x000f0000) >> 16;
      // Setup the base
      gdt[seg].bits.base_lo   = base & 0x00ffffff;
      gdt[seg].bits.base_hi   = (base & 0xff000000) >> 24;
    };

  public:
    enum
    {
      seg_NULL = 0,         ///< \brief Null segment
      seg_KernelCS,         ///< \brief Kernel's code segment
      seg_KernelDS,         ///< \brief Kernel's data segment
      seg_UserCS,           ///< \brief User's code segment
      seg_UserDS,           ///< \brief User's data segment
      seg_UserTLS,          ///< \brief User's thread-local storage
      seg_TSS,              ///< \brief Task-switch segment
      seg_Count             ///< \brief Number of segments + 1
  };

  protected:
    union SegmentDescriptor_t
    {
      struct
      {
        uint64_t  limit_lo    : 16; ///< \brief Segment limit (low)
        uint64_t  base_lo     : 24; ///< \brief Base address (low)
        uint64_t  type        : 4;  ///< \brief Segment type
        uint64_t  system      : 1;  ///< \brief Descriptor type (0=system, 1=code|data)
        uint64_t  dpl         : 2;  ///< \brief Descriptor privilege level
        uint64_t  present     : 1;  ///< \brief Segment present
        uint64_t  limit_hi    : 4;  ///< \brief Segment limit (high)
        uint64_t  available   : 1;  ///< \brief Available for system programmer's use
        uint64_t  l           : 1;  ///< \brief 64bit segment (IA-32e mode only, else 0)
        uint64_t  opSize      : 1;  ///< \brief Operation size (0=16bit, 1=32bit)
        uint64_t  granularity : 1;  ///< \brief Granularity (0=byte, 1=4kb)
        uint64_t  base_hi     : 8;  ///< \brief Base address (high)
      } bits;                       ///< \brief Segment descriptor bits
      uint64_t        value;        ///< \brief Segment descriptor 64bit value
    };

    enum TypeField_t
    {
      // Data segment
      Data        = 0x0,  // just for clarity
      Accessed    = 0x1,  ///< \brief The CPU sets this bit when segment is accessed
      Writable    = 0x2,  ///< \brief Writable segment (a must for Stack)
      ExpandDown  = 0x4,
      // Code segment
      Code        = 0x8,
      //Accessed  = 0x1,  // the same as in data segment
      Readable    = 0x2,  ///< @note If data is in the code segment, it must be readable
      Conforming  = 0x4,  ///< @warning Transfer into non-conforming at different privilege => #GP exception!
      // System descriptor types
      // reserved = 0x0,
      TSS16avl    = 0x1,  ///< \brief 16bit TSS (available)
      LDT         = 0x2,  ///< \brief Local descriptor table
      TSS16busy   = 0x3,  ///< \brief 16bit TSS (busy)
      CallGate16  = 0x4,  ///< \brief 16bit call gate
      TaskGate    = 0x5,  ///< \brief Task gate
      IntGate16   = 0x6,  ///< \brief 16bit interrupt gate
      TrapGate16  = 0x7,  ///< \brief 16bit trap gate
      // reserved = 0x8,
      TSS32avl    = 0x9,  ///< \brief 32bit TSS (available)
      // reserved = 0xA,
      TSS32busy   = 0xB,  ///< \brief 32bit TSS (busy)
      CallGate32  = 0xC,  ///< \brief 32bit call gate
      // reserved = 0xD,
      IntGate32   = 0xE,  ///< \brief 32bit interrupt gate
      TrapGate32  = 0xF,  ///< \brief 32bit trap gate
  };

    union GDTR_t
    {

      struct
      {
        uint64_t  limit   : 16; ///< \brief 16bit limit
        uint64_t  address : 32; ///< \brief 32bit address
        uint64_t  nothing : 16; ///< \brief 16bit nothing and not used
      } bits;                   ///< \brief GDTR bits
      uint64_t value;           ///< \brief 64bit value of gdtr
    };

    /// \brief Global descriptor table
    SegmentDescriptor_t * gdt;

    /// \brief Global descriptor table register
    GDTR_t * gdtr;
};

#endif /* __GDT_H__ */
