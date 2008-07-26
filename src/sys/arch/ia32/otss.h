/*
 *  Copyright (C) 2008 by Filip Brcic <brcha@gna.org>
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
/** @file otss.h
 * @brief Description
 */
#ifndef OTSS_H
#define OTSS_H

#include <config.h>
#include <types.h>

class OGDT;

/**
 * @brief Task-state segment
 */
class OTSS{
  public:
    /**
     * @brief Get one instance of this class
     */
    static OTSS * instance();

    /**
     * @brief Initialize the TSS
     */
    void initialize();
    
    /**
     * @brief Load TSS into the TR
     */
    void load();
  protected:
    /**
     * @brief Protected constructor
     */
    OTSS();
    
  protected:
    struct TSS_t
    {
      uint32_t    backlink;
      uint32_t    esp0;
      uint32_t    ss0;
      uint32_t    esp1;
      uint32_t    ss1;
      uint32_t    esp2;
      uint32_t    ss2;
      uint32_t    cr3;
      uint32_t    eip;
      uint32_t    eflags;
      uint32_t    eax;
      uint32_t    ecx;
      uint32_t    edx;
      uint32_t    ebx;
      uint32_t    esp;
      uint32_t    ebp;
      uint32_t    esi;
      uint32_t    edi;
      uint32_t    es;
      uint32_t    cs;
      uint32_t    ss;
      uint32_t    ds;
      uint32_t    fs;
      uint32_t    gs;
      uint32_t    ldtr;
      uint16_t    trapOnSwitch; /* this is in fact boolean = 0 or 1 */
      uint16_t    ioMapBase;    /* should be = sizeof(TSS_t) */
    };

    TSS_t m_tss[MAX_NCPU];
    
    friend class OGDT;
};

#endif
