#ifndef __Paging_H__
#define __Paging_H__
/*
 *  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file Paging.h
 * \brief Paging support
 *
 * \todo Is this file IA32 specific or generic???
 */
/*
 * $Id$
 */

#include <types.h>

/**
 * \brief Paging base class
 *
 * This is the base class for paging support. It has to be inherited and implemented
 * for specific architecture and specific paging method.
 */

class Paging
{
  public:
    /**
     * \brief Setup paging
     *
     * After booting, the kernel is located at physical memory location 0x00100000,
     * but kernel is relocated at 0xC0100000 (= KVA + 0x00100000 = 3 GB + 1 MB).
     *
     * This method should map the physical memory [0, 2M] to virtual [0, 2M] and
     * also to [KVA, KVA+2M].
     *
     * In case of legacy mapping, it will map [0, 4M] to virtual [0, 4M] and KVA + [0, 4M].
     */
    virtual void setup()
    {
      fatal(ANSI_FG_RED "Paging::setup is virtual function!!!\n" ANSI_NORMAL);
    };

  protected:
    /**
     * \brief Inhibit creating instances of this class
     */
    Paging() {};

    /**
     * \brief Get the CR0 register
     * \returns value of the CR0 register
     */
    uint32_t cr0();
    /**
     * \brief Set the CR0 register
     * \param value new value for the CR0 register
     *
     * \warning This method enables paging (if PG bit is set), so take care how you use it
     */
    void cr0(uint32_t value);

    /**
     * \brief Get the CR3 register
     * \returns value of the CR3 register
     */
    uint32_t cr3();
    /**
     * \brief Set the CR3 register
     * \param value new value for CR3 register
     */
    void cr3(uint32_t value);

    /**
     * \brief Get the CR4 register
     * \returns the value from CR4
     */
    uint32_t cr4();
    /**
     * \brief Set the CR4 register
     * \param value new value for CR4 register
     */
    void cr4(uint32_t value);
};

/**
 * \brief Enable debugging messages or not
 */
#define DEBUG_PAGING DEBUG_ENABLE

#endif /* __Paging_H__ */
