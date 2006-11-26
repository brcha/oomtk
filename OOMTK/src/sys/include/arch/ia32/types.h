#ifndef __ARCH__IA32__TYPES_H__
#define __ARCH__IA32__TYPES_H__
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/*
 * $Id: types.h 195 2006-10-22 20:25:10Z brcha $
 */
/** @file
 * @brief Basic types for IA32
 */

typedef unsigned int __attribute__((__mode__(__DI__))) u64_t; // or unsigned long long int
typedef unsigned int      u32_t;
typedef unsigned short    u16_t;
typedef unsigned char     u8_t;

typedef signed int __attribute__((__mode__(__DI__))) s64_t;
typedef signed int        s32_t;
typedef signed short      s16_t;
typedef signed char       s8_t;

/**
 * @brief Machine word wide unsigned int
 */
typedef u32_t             word_t;

/// @brief How many bits does machine word have?
enum {
  WORD_BITS = 32
};

/**
 * @defgroup OtherNames
 * @brief Other common names for previous types
 * @{
 */
typedef u32_t   ulong;
typedef u32_t   u_long;
typedef u16_t   uint;
typedef u16_t   u_int;
typedef u8_t    uchar;
typedef u8_t    u_char;

// The following names are best, since I've got syntax highlight for them
// And they are kind-of in the standard :)
typedef u32_t   uint32_t;
typedef s32_t   int32_t;
typedef u16_t   uint16_t;
typedef s16_t   int16_t;
typedef u8_t    uint8_t;
typedef s8_t    int8_t;
/** @} */

/**
 * @defgroup PrintfFormats
 * @brief Printf formats for various types
 * @{
 */
#define OOMTK_PTR_FMT         "%08lx"
#define OOMTK_MWORD_FMT       "%08lx"
#define OOMTK_X64_FMT         "%016llx"
#define OOMTK_ADDR_INPUT_FMT  "%8x"
#define OOMTK_FRAME_INPUT_FMT "%5x"

#endif /* __ARCH__IA32__TYPES_H__ */
