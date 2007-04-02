#ifndef __VgaConsole_H__
#define __VgaConsole_H__
/*
 *  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file VgaConsole.h
 * \brief VGA Console
 */
/*
 * $Id$
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
