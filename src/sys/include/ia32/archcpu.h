#ifndef __IA32_ARCHCPU_H__
#define __IA32_ARCHCPU_H__
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
/** @file archcpu.h
 * @brief Architecture dependant cpu structure
 */
#include <types.h>

struct arch_cpu_t
{
  kpa_t localDataPageTable;
  kpa_t transMapMappingPage;
  
  uint8_t lapic_id;
};

#endif /* __IA32_ARCHCPU_H__ */
