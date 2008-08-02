#ifndef __IA32_OPAGING_H__
#define __IA32_OPAGING_H__
/*
 *  Copyright (C) 2006-2008 by Filip Brcic <brcha@gna.org>
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
 * @brief Paging support
 *
 * @todo Is this file IA32 specific or generic???
 */

#include <types.h>
#include <fatal.h>
#include <ansi.h>

#include <OOMTK/OPaging>

extern "C" bool UsingPAE;

/**
 * @brief Paging base class
 *
 * This is the base class for paging support. It has to be inherited and implemented
 * for specific architecture and specific paging method.
 */
class OPagingIA32 : public OPaging
{
  public:
    /**
   * @brief Get one instance of OPagingIA32 class
   * 
   * Actually, this returns the instance of OPagingLegacy or OPagingPAE class, depending on
   * the value of bool UsingPAE set in start.S
     */
    static OPagingIA32 * instance();
    
    /**
     * @brief Setup paging
     *
     * After booting, the kernel is located at physical memory location 0x00100000,
     * but kernel is relocated at 0xC0100000 (= KVA + 0x00100000 = 3 GB + 1 MB).
     *
     * This method should map the physical memory [0, 2M] to virtual [0, 2M] and
     * also to [KVA, KVA+2M].
     *
     * In case of legacy mapping, it will map [0, 4M] to virtual [0, 4M] and KVA + [0, 4M].
     */
    virtual void setup() = 0;
    
  protected:
    /**
   * @brief Inhibit creating instances of this class
     */
    OPagingIA32() {}
    virtual ~OPagingIA32() {}

    /**
     * @brief Get the CR0 register
     * @returns value of the CR0 register
     */
    uint32_t cr0();
    /**
     * @brief Set the CR0 register
     * @param value new value for the CR0 register
     *
     * @warning This method enables paging (if PG bit is set), so take care how you use it
     */
    void cr0(uint32_t value);

    /**
     * @brief Get the CR3 register
     * @returns value of the CR3 register
     */
    uint32_t cr3();
    /**
     * @brief Set the CR3 register
     * @param value new value for CR3 register
     */
    void cr3(uint32_t value);

    /**
     * @brief Get the CR4 register
     * @returns the value from CR4
     */
    uint32_t cr4();
    /**
     * @brief Set the CR4 register
     * @param value new value for CR4 register
     */
    void cr4(uint32_t value);
};

#endif /* __IA32_OPAGING_H__ */
