#ifndef __OOMTKSYS_OPORTIO_H__
#define __OOMTKSYS_OPORTIO_H__
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
/** \file
 * \brief Abstract Port I/O API
 *
 * NOTE: This class must have architecture specific implementations!
 */
#include <types.h>

/**
 * Ports input/output API
 */
class OPortIO
{
  public:
    /**
     * Delay for slow i/o operations
     */
    static void iodelay();

    /**
     * @defgroup Normal
     * @brief Regular port i/o operations
     * @{
     */
    /**
     * @brief Read from 8bit port
     */
    static uint8_t in8(word_t port);

    /**
     * @brief Read from 16bit port
     */
    static uint16_t in16(word_t port);

    /**
     * @brief Read from 32bit port
     */
    static uint32_t in32(word_t port);

    /**
     * @brief Write to 8bit port
     */
    static void out8(word_t port, uint8_t value);

    /**
     * @brief Write to 16bit port
     */
    static void out16(word_t port, uint16_t value);

    /**
     * @brief Write to 32bit port
     */
    static void out32(word_t port, uint32_t value);
    /** @} */

    /**
     * \defgroup Delayed
     * \brief Delayed port i/o operations
     * @{
     */
    /**
     * @brief Read from 8bit port
     */
    static uint8_t in8_d(word_t port);

    /**
     * @brief Read from 16bit port
     */
    static uint16_t in16_d(word_t port);

    /**
     * @brief Read from 32bit port
     */
    static uint32_t in32_d(word_t port);

    /**
     * @brief Write to 8bit port
     */
    static void out8_d(word_t port, uint8_t value);

    /**
     * @brief Write to 16bit port
     */
    static void out16_d(word_t port, uint16_t value);

    /**
     * @brief Write to 32bit port
     */
    static void out32_d(word_t port, uint32_t value);
    /** @} */
};

#endif /* __OOMTKSYS_OPORTIO_H__ */
