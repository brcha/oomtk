INTERFACE:
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** @file
 * @brief Segment Descriptor
 *
 * For more information on segment descriptors see Intel's "IA-32 Intel
 * Architecture Software Developer's Manual, Volume 3A: System Programming Guide"
 * (don't know the exact number of the guide, but it is the fourth PDF that can
 * be downloaded from Intel's website. My advice is download them all and rename
 * them after their titles, like I did)
 */

#include <types.h>

/**
 * @brief Segment descriptor data structure
 *
 * This is not a class because I am not fully sure how whould CPU react on loading
 * classes into segment table registers :) It is better (or at least safer) to leave
 * this as is.
 */
union SegmentDescriptor {
  struct {
    // this is the lower u32_t
    u32_t loLimit       :16;  ///< @brief Lower part of the segment limit (15:00)
    u32_t loBase        :16;  ///< @brief Lower part of the base address (15:00)
    // this is the upper u32_t
    u32_t midBase       : 8;  ///< @brief Middle part of the base address (23:16)
    u32_t type          : 4;  ///< @brief Segment type
    u32_t system        : 1;  ///< @brief Descriptor type (0 = system, 1 = code/data)
    u32_t dpl           : 2;  ///< @brief Descriptor privilege level (0, 1, 2 or 3)
    u32_t present       : 1;  ///< @brief Present bit
    u32_t hiLimit       : 4;  ///< @brief Upper part of segment limit (19:16)
    u32_t avl           : 1;  ///< @brief Available for use by system software
    u32_t l             : 1;  ///< @brief 64bit code segment (IA-32e mode only) = 0
    u32_t size          : 1;  ///< @brief Default op. size (0 = 16bit, 1 = 32bit)
    u32_t granularity   : 1;  ///< @brief Granularity bit
    u32_t hiBase        : 8;  ///< @brief Upper part of the base address (31:24)
  } bits;                     ///< @brief Segment descriptor bits
  struct {
    u32_t lower;              ///< @brief Lower word of the segment descriptor
    u32_t upper;              ///< @brief Upper word of the segment descriptor
  } words;                    ///< @brief Segment descriptor 32bit words
  u64_t value;                ///< @brief Full segment descriptor value - 64bit
};

/**
 * @brief Global descriptor table
 */
DECLARE_PER_CPU(SegmentDescriptor[], GDT);

/**
 * @defgroup SegmentSelectors
 * @brief Segment selectors
 *
 * Segment selector =
 *   Index            - bits 15:3
 *   Table Indicator  - bit 2 (0 = GDT, 1 = LDT)
 *   RPL              - bits 1:0
 * @{
 */
/// @brief Number of segments
#define seg_Count     7
// Segment indexes
/// @brief The NULL segment index
#define seg_NULL      0x0
/// @brief The TSS (Task-State Segment) segment index
#define seg_TSS       0x1
// Intel SYSENTER/SYSRETURN require Kernel CS, DS, User CS, DS order
/// @brief Kernel code segment index
#define seg_KernelCS  0x2
/// @brief Kernel data segment index
#define seg_KernelDS  0x3
/// @brief User code segment index
#define seg_UserCS    0x4
/// @brief User data segment index
#define seg_UserDS    0x5
/// @brief User thread local storage segment index
#define seg_UserTLS   0x6

/// @brief Segment selector = segment index*8 + RPL
#define SELECTOR(seg, rpl)  ((seg*8)+rpl)

/// @brief NULL segment selector
#define sel_NULL      SELECTOR(seg_NULL, 0)
/// @brief TSS segment selector
#define sel_TSS       SELECTOR(seg_TSS, 0)
/// @brief Kernel code segment selector
#define sel_KernelCS  SELECTOR(seg_KernelCS, 0)
/// @brief Kernel data segment selector
#define sel_KernelDS  SELECTOR(seg_KernelDS, 0)
/// @brief User code segment selector
#define sel_UserCS    SELECTOR(seg_UserCS, 3)
/// @brief User data segment selector
#define sel_UserDS    SELECTOR(seg_UserDS, 3)
/// @brief User thread local storage segment index
#define sel_UserTLS   SELECTOR(seg_UserTLS, 3)
/** @} */ // SegmentSelectors group

/**
 * @defgroup SegmentType
 * @brief Segment type definitions
 * @{
 */
/// @brief Accessed field
#define segType_A     0x1
/// @brief Read/Write field (for data segments)
#define segType_W     0x2
/// @brief Expand-down field (for data segments)
#define segType_E     0x4
/// @brief Read-enabled field (for code segments)
#define segType_R     0x2
/// @brief Conforming field (for code segments)
#define segType_C     0x4
/// @brief Code segment (1 = code, 0 = data)
#define segType_Code  0x8
/// @brief Data segment
#define segType_Data  0x0
/** @} */ // SegmentType group

IMPLEMENTATION:

DEFINE_PER_CPU(SegmentDescriptor[seg_Count], GDT);

/**
 * @brief Setup the segment limit and base
 */
inline void gdt_setupLimitBase(u32_t segment, u32_t limit, u32_t base)
{
  // Setup the limit
  GDT[segment].bits.loLimit = limit & 0x0000ffff;
  GDT[segment].bits.hiLimit = (limit & 0x000f0000) >> 16;
  // Setup the base
  GDT[segment].bits.loBase = base & 0x0000ffff;
  GDT[segment].bits.midBase = (base & 0x00ff0000) >> 16;
  GDT[segment].bits.hiBase = (base & 0xff000000) >> 24;
}

struct GDT_Register {
  u32_t limit     : 16;   ///< @brief Size of GDT
  u32_t loAddress : 16;   ///< @brief Lower part of GDT address
  u32_t hiAddress : 16;   ///< @brief Upper part of GDT address
};

GDT_Register gdtr = {
  sizeof(GDT),
  reinterpret_cast<u32_t> (GDT) - KVA,
  (reinterpret_cast<u32_t> (GDT) - KVA )>> 16
};

/**
 * @brief Setup the Global Descriptor Table
 */
void SetupGDT()
{
  // Null segment
  GDT[seg_NULL].value = 0;

  // TSS segment - don't know anything about this now... will be setup later.
  // I will setup just some data now
  //gdt_setupLimitBase(seg_TSS, sizeof(TSS), 0); -- when I make TSS struct :)
  // this is taken from Coyotos, but I am not sure if this should look like this
  GDT[seg_TSS].bits.type        = segType_Code | segType_A ;
  GDT[seg_TSS].bits.system      = 0; // system segment
  GDT[seg_TSS].bits.dpl         = 0; // ring0
  GDT[seg_TSS].bits.present     = 1;
  GDT[seg_TSS].bits.avl         = 0;
  GDT[seg_TSS].bits.l           = 0; // this is always 0
  GDT[seg_TSS].bits.size        = 0; // 16bit???
  GDT[seg_TSS].bits.granularity = 0; // byte granularity

  // Kernel Code Segment
  // Based at 0, size = 4 GB (max)
  gdt_setupLimitBase(seg_KernelCS, 0x000fffff, 0);
  GDT[seg_KernelCS].bits.type         = segType_Code | segType_R | segType_A;
  GDT[seg_KernelCS].bits.system       = 1; // code segment => !system :)
  GDT[seg_KernelCS].bits.dpl          = 0; // ring0 (or could it be ring3?)
  GDT[seg_KernelCS].bits.present      = 1;
  GDT[seg_KernelCS].bits.avl          = 0;
  GDT[seg_KernelCS].bits.l            = 0;
  GDT[seg_KernelCS].bits.size         = 1; // 32bit
  GDT[seg_KernelCS].bits.granularity  = 1; // 4k granularity

  // Kernel Data Segment
  // Based at 0, size = 4 GB (max)
  gdt_setupLimitBase(seg_KernelDS, 0x000fffff, 0);
  GDT[seg_KernelDS].bits.type         = segType_Data | segType_W | segType_A;
  GDT[seg_KernelDS].bits.system       = 1; // data segment => !system :)
  GDT[seg_KernelDS].bits.dpl          = 0; // ring0 (or could it be ring3?)
  GDT[seg_KernelDS].bits.present      = 1;
  GDT[seg_KernelDS].bits.avl          = 0;
  GDT[seg_KernelDS].bits.l            = 0;
  GDT[seg_KernelDS].bits.size         = 1; // 32bit
  GDT[seg_KernelDS].bits.granularity  = 1; // 4k granularity

  // User Code Segment
  // Based at 0, size = 4 GB (max)
  gdt_setupLimitBase(seg_UserCS, 0x000fffff, 0);
  GDT[seg_UserCS].bits.type           = segType_Code | segType_R | segType_A;
  GDT[seg_UserCS].bits.system         = 1; // code segment => !system :)
  GDT[seg_UserCS].bits.dpl            = 3; // ring3
  GDT[seg_UserCS].bits.present        = 1;
  GDT[seg_UserCS].bits.avl            = 0;
  GDT[seg_UserCS].bits.l              = 0;
  GDT[seg_UserCS].bits.size           = 1; // 32bit
  GDT[seg_UserCS].bits.granularity    = 1; // 4k granularity

  // User Data & Stack Segment
  // Based at 0, size = 4 GB (max)
  gdt_setupLimitBase(seg_UserDS, 0x000fffff, 0);
  GDT[seg_UserDS].bits.type           = segType_Data | segType_W | segType_A;
  GDT[seg_UserDS].bits.system         = 1; // data segment => !system :)
  GDT[seg_UserDS].bits.dpl            = 3; // ring3
  GDT[seg_UserDS].bits.present        = 1;
  GDT[seg_UserDS].bits.avl            = 0;
  GDT[seg_UserDS].bits.l              = 0;
  GDT[seg_UserDS].bits.size           = 1; // 32bit
  GDT[seg_UserDS].bits.granularity    = 1; // 4k granularity

  // User Thread Local Storage
  // Size = 4 GB (max), Base is variable
  gdt_setupLimitBase(seg_UserTLS, 0x000fffff, 0);
  GDT[seg_UserTLS].bits.type          = segType_Data | segType_W | segType_A;
  GDT[seg_UserTLS].bits.system        = 1; // data segment => !system :)
  GDT[seg_UserTLS].bits.dpl           = 3; // ring3
  GDT[seg_UserTLS].bits.present       = 1;
  GDT[seg_UserTLS].bits.avl           = 0;
  GDT[seg_UserTLS].bits.l             = 0;
  GDT[seg_UserTLS].bits.size          = 1; // 32bit
  GDT[seg_UserTLS].bits.granularity   = 1; // 4k granularity

  // Good, now go and load GDT into GDTR

  void * gdtrPhys = reinterpret_cast<void *> (reinterpret_cast<u32_t>(&gdtr) - KVA);
  GNU_ASM(
      "   lgdt  %0          \n"   // Load gdtr
      "   ljmp  %1, $1f     \n"   // Reload %eip with sel_KernelCS
      "1: mov   %2, %%eax   \n"   // Reload data segments with sel_KernelDS
      "   mov   %%ax, %%ds  \n"
      "   mov   %%ax, %%es  \n"
      "   mov   %%ax, %%ss  \n"
      "   xor   %%ax, %%ax  \n"
      "   mov   %%ax, %%fs  \n"   // Reload %fs and %gs with 0
      "   mov   %%ax, %%gs  \n"
      "   lldt  %%ax        \n"   // Load LDT
  : /* no output */
  : "m" (gdtrPhys), "i" (sel_KernelCS), "i" (sel_KernelDS)
  : "memory", "ax"
         );
};
