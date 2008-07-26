#ifndef __MACROS_H__
#define __MACROS_H__
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@gna.org>
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
/** @file
 * @brief Macro definitions for convenience
 */

#include <config.h>

/**
 * Cache align data
 */
#define CACHE_ALIGN __attribute__((__aligned__(CACHE_LINE_SIZE), \
                                   __section__(".data.cachealign")))

/**
 * Page align data
 */
#define PAGE_ALIGN __attribute__((__aligned__(PAGE_SIZE), \
                                  __section__(".pagedata")))


/* Macros for code/data placement */
#define SECTION(x)    __attribute__((section(x)))
#define UNIT(x)       __attribute__((section(".data." x)))

/**
 * @brief WEAK function can be replaced by arch specific optimization
 */
#define WEAK          __attribute__(( weak ))

/**
 * @brief Marks a function as noreturn
 */
#define NORETURN      __attribute__(( noreturn ))

/**
 * @brief Marks a function as not to be generated inline
 */
#if (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1)
# define NOINLINE     __attribute__(( noinline ))
#else
# define NOINLINE
#endif

/**
 * @brief Marks variables as unused
 */
#define UNUSED __attribute__(( unused ))

/**
 * @brief Marks the function whose parameters are passed through registers
 */
#ifdef __i386__
#define FASTCALL	__attribute__((regparm(3)))
#else
#define FASTCALL
#endif

/**
 * @brief Marks the function as const
 */
#define CONST           __attribute__((__const__))

/**
 * @brief Functions with this are NEVER generated standalone.
 *
 * They are inlined only.
 *
 * @b Carefull: If the maximum inlining limit is reached, gcc 3.x does not
 * inline even if explicitly specified. Use -finline-limit=<large> here
 */
#define INLINE extern inline

/* Functions for critical path optimization */
#if (__GNUC__ >= 3)
#define EXPECT_FALSE(x)     __builtin_expect((x), false)
#define EXPECT_TRUE(x)      __builtin_expect((x), true)
#define EXPECT_VALUE(x,val) __builtin_expect((x), (val))
#else /* __GNUC__ < 3 */
#define EXPECT_FALSE(x)     (x)
#define EXPECT_TRUE(x)      (x)
#define EXPECT_VALUE(x,val) (x)
#endif

/* Convenience functions for memory sizes */
#define KB(x)               ((typeof (x)) ((word_t) x * 1024))
#define MB(x)               ((typeof (x)) ((word_t) x * 1024*1024))
#define GB(x)               ((typeof (x)) ((word_t) x * 1024*1024*1024))

/* Other convenience functions  */
#define max(a,b)            ((a) >? (b))
#define min(a,b)            ((a) <? (b))

/* Turn preprocessor symbol definition into a string */
#define MKSTR(sym)    MKSTR2(sym)
#define MKSTR2(sym)   #sym

/* Safely "append" an Unsigned-Long suffix for an asm and C value */
#ifdef ASSEMBLY
#define __UL(x)       x
#else
#define __UL(x)       x##UL
#endif

/* Use inline assembler */
#define GNU_ASM(...) __asm__ __volatile__(__VA_ARGS__)

/* Used in C library to make it able to link with C and C++ */
#ifndef __cplusplus
#define __BEGIN_DECLS
#define __END_DECLS
#else
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#endif

#define def_to_string(s) #s

/**
 * @brief Enable debugging messages
 *
 * @note Optimization will remove the if(1).
 */
#define DEBUG_ENABLE  if(1)
/**
 * @brief Disable debugging messages
 *
 * @note Optimization will remove if(0) and the following command.
 */
#define DEBUG_DISABLE if(0)

/**
 * @brief Declare the class friendly to the function that determines offsets of members
 */
#define OFFSET_FRIENDLY friend void __generateOffsets__(void)

/**
 * @brief Declare exported function
 */
#ifndef OOMTKSYS_EXPORT
#define OOMTKSYS_EXPORT
#endif

/**
 * @defgroup KVA
 * @brief Virtual to physical address conversion and back.
 * @{
 */
#define KVTOL(a)      (a)
#define PTOKV(a, T)   ((T) ((kva_t) (((kpa_t)(a)) + KVA)))
#define KVTOP(a)      ((kpa_t) (KVTOL((kva_t)(a)) - KVA))
/** @} */

/**
 * @brief Sets the whole structure/class to 0
 * 
 * @note Remember to include <string.h> before using this macro.
 */
#define INIT_TO_ZERO(ob) memset(ob, 0, sizeof(__typeof__(*ob)))

#endif /* __MACROS_H__ */
