#ifndef __OOMTKSYS_OVGACONSOLE_H__
#define __OOMTKSYS_OVGACONSOLE_H__
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
/** \file
 * \brief VGA Console
 */
#include <types.h>
#include <ansi.h>
#include "oconsole.h"

/**
 * VGA Console
 *
 * This class implements an output-only console for VGA device.
 * It implements some ANSI Escape codes, too.
 */

class OVgaConsole : public OConsole
{
  public:
    /**
     * @brief One instance of OVgaConsole
     * @returns the instance
     */
    static OVgaConsole * instance();
    
    /**
     * @brief Initialize the console
     */
    void initialize();

    void putchar(char c);

    /**
     * Clear the screen
     */
    void clear();

    virtual word_t attributes() const;

    virtual int write(const char * string, size_t length);

    void setColor(uint8_t color);
    
    /**
     * @brief Scroll mode
     * @param mode = {ScrollMode::Normal, ScrollMode::Emacs}
     */
    void setScrollMode(uint8_t mode);

  protected:
    /**
     * @brief Protected constructor -- not for use by mortals
     */
    OVgaConsole();
    
    /**
     * @brief Set char at the position
     * @param pos the position
     * @param c the character
     */
    void setAt(uint32_t pos, char c);

    /**
     * @brief Scroll the screen
     * @param start line of the area
     * @param end line of the area
     * @param amount of lines to scroll
     */
    void scroll(uint32_t start, uint32_t end, int amount);
    
    /**
     * @brief Beep the PC speaker
     * @todo This could actually do something...
     */
    void beep() {}

    /**
     * @brief Show the cursor at the position
     * @param pos the position of the cursor
     */
    void showCursorAt(uint32_t pos);

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
      
      WhiteOnBlack  = 0x7, /* the most common setting */
    };
    
    enum ScrollMode
    {
      Normal,
      Emacs,
    };

  private:
    volatile uint16_t * m_display;    ///< @brief The display
    uint8_t   m_color;                ///< @brief Current color
    uint8_t   m_newColor;             ///< @brief New color (ANSI Escape)
    uint8_t   m_esc;                  ///< @brief Is escape parsed?
    uint8_t   m_escArg;               ///< @brief Argument for escape
    uint8_t   m_columns;              ///< @brief Number of columns
    uint8_t   m_lines;                ///< @brief Number of lines (could be less than above)
    uint32_t  m_startAddressRegister; ///< @brief Start address register
    uint32_t  m_offset;               ///< @brief Position on the display
    uint8_t   m_scrollMode;           ///< @brief Scroll mode = {Normal, <b>Emacs</b>}
};

#endif /* __OOMTKSYS_OVGACONSOLE_H__ */
