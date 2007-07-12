#ifndef __VgaConsole_H__
#define __VgaConsole_H__
/*
 *  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK (http://launchpad.net/oomtk)
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
/** \file VgaConsole.h
 * \brief VGA Console
 */
#include <types.h>
#include <Console.h>
#include <ansi.h>

/**
 * VGA Console
 *
 * This class implements an output-only console for VGA device.
 * It implements some ANSI Escape codes, too.
 */

class VgaConsole : public Console
{
  public:
    /**
     * \brief One instance of VgaConsole
     * @returns the instance
     */
    static VgaConsole * instance();

    void putchar(char c);

    /**
     * Clear the screen
     */
    void clear();

    virtual word_t attributes() const;

    virtual int write(const char * string, size_t length);

    void setColor(u8_t color);

  protected:
    /**
     * \brief Protected constructor -- not for use by mortals
     */
    VgaConsole();

    void scroll();

    void moveCursor();

  public:
    enum
    {
      Black         =  0,
      Blue          =  1,
      Green         =  2,
      Cyan          =  3,
      Red           =  4,
      Magenta       =  5,
      Brown         =  6,
      LightGrey     =  7,
      DarkGrey      =  8,
      LightBlue     =  9,
      LightGreen    = 10,
      LightCyan     = 11,
      LightRed      = 12,
      LightMagenta  = 13,
      LightBrown    = 14,
      White         = 15,
    };

  private:
    u16_t * m_display;    ///< The display

    u8_t    m_x;          ///< Current X coordinate

    u8_t    m_y;          ///< Current Y coordinate

    u8_t    m_color;      ///< Current color

    u8_t    m_newColor;   ///< New color (ANSI Escape)

    u8_t    m_esc;        ///< Is escape parsed?

    u8_t    m_escArg;     ///< Argument for escape

    u8_t    m_columns;    ///< Number of columns

    u8_t    m_lines;      ///< Number of lines
};

#endif /* __VgaConsole_H__ */
