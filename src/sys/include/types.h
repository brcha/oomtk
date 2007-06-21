#ifndef __TYPES_H__
#define __TYPES_H__
/*
 *  Copyright (C) 1994-2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK v.2
 */
/*
 * $Id: types.h 195 2006-10-22 20:25:10Z brcha $
 */
/** @file
 * @brief Type definitions
 */

#ifndef __ASSEMBLY__

#include INC_ARCH(types.h)

/**
 * @brief Data type to store addresses
 */
typedef void* addr_t;

#ifdef __cplusplus
/**
 * @brief Add offset to address
 * @param addr original address
 * @param off offset to add
 * @return new address
 */
INLINE addr_t addr_offset(addr_t addr, addr_t off)
{
  return (addr_t) ((word_t)addr + (word_t)off);
}
#endif

/**
 * @brief Add offset to address
 * @param addr original address
 * @param off offset to add
 * @return new address
 */
INLINE addr_t addr_offset(addr_t addr, word_t off)
{
  return (addr_t) ((word_t)addr + off);
}

/**
 * @brief Apply mask to an address
 * @param addr original address
 * @param mask address mask
 * @return new address
 */
INLINE addr_t addr_mask (addr_t addr, word_t mask)
{
  return (addr_t) ((word_t) addr & mask);
}

/**
 * @brief Align address downwards.
 * It is assumed that the alignment is a power of 2.
 * @param addr original address
 * @param align alignment
 * @return new address
 */
INLINE addr_t addr_align (addr_t addr, word_t align)
{
  return addr_mask (addr, ~(align - 1));
}

/**
 * @brief Align address upwards.
 * It is assumed that the alignment is a power of 2.
 * @param addr original address
 * @param align alignment
 * @return new address
 */
INLINE addr_t addr_align_up (addr_t addr, word_t align)
{
  return addr_mask (addr_offset (addr, align - 1), ~(align - 1));
}

#ifndef NULL
#define NULL 0
#endif

#endif /* ! __ASSEMBLY__ */

#endif /* __TYPES_H__ */
