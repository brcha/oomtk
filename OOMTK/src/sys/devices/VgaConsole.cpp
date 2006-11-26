INTERFACE:
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/*
 * $Id: VgaConsole.cpp 196 2006-10-23 13:29:52Z brcha $
 */
/** @file
 * @brief VGA Console
 */

#include <types.h>
#include <Console.h>

/**
 * VGA Console
 *
 * This class implements an output-only console for VGA device.
 * It implements some ANSI Escape codes, too.
 */
class VgaConsole : public Console
{
  public:
    enum {
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

/**
 * @defgroup ANSI
 * @brief These are some defines that make using these limited ANSI macros easier
 */
#define ANSI_NORMAL       "\e[0m"
#define ANSI_BRIGHT       "\e[1m"
#define ANSI_REVERSED     "\e[2m"
#define ANSI_CLEAR        "\e[2J"
#define ANSI_FG_BLACK     "\e[30m"
#define ANSI_FG_RED       "\e[31m"
#define ANSI_FG_GREEN     "\e[32m"
#define ANSI_FG_YELLOW    "\e[33m"
#define ANSI_FG_BLUE      "\e[34m"
#define ANSI_FG_MAGENTA   "\e[35m"
#define ANSI_FG_CYAN      "\e[36m"
#define ANSI_FG_WHITE     "\e[37m"
#define ANSI_BG_BLACK     "\e[40m"
#define ANSI_BG_RED       "\e[41m"
#define ANSI_BG_GREEN     "\e[42m"
#define ANSI_BG_YELLOW    "\e[43m"
#define ANSI_BG_BLUE      "\e[44m"
#define ANSI_BG_MAGENTA   "\e[45m"
#define ANSI_BG_CYAN      "\e[46m"
#define ANSI_BG_GREY      "\e[47m"
/** @} */

IMPLEMENTATION:

#include <cstring>
#include <cctype>

#include <PortIO.h>

/**
 * @brief One instance of VgaConsole
 * @returns the instance
 */
PUBLIC static VgaConsole * VgaConsole::instance()
{
  static VgaConsole _instance = VgaConsole();

  return &_instance;
};

/**
 * @brief Protected constructor -- not for use by mortals
 */
PROTECTED VgaConsole::VgaConsole()
{
  m_display = (u16_t *) (KVA + 0xB8000); //0xF00B8000;
  m_x = 0;
  m_y = 0;
  m_color = LightGrey;
  m_newColor = 0;
  m_esc = 0;
  m_escArg = 0;
  m_columns = 80;
  m_lines = 25;
}

PUBLIC void VgaConsole::putchar(char c)
{
  static const u8_t colors[] = {0, 4, 2, 14, 1, 5, 3, 15};

  if (m_esc == 1)
  {
    if (c == '[')
    {
      m_esc ++;
      return;
    }
  } else if (m_esc == 2)
  {
    switch (c)
    {
      case '0': case '1': case '2':
      case '3': case '4': case '7':
        m_esc ++;
        m_escArg = c;
        return;
    }
  } else if (m_esc == 3)
  {
    switch (c)
    {
      case '0': case '1': case '2': case '3':
      case '4': case '5': case '6': case '7':
        if (m_escArg == '3' || m_escArg == '4')
        {
          // Set the new foreground or background color
          m_newColor = colors[c-'0'];
          m_esc ++;
          return;
        }
        break;
      case 'J':
        if (m_escArg == '2')
        {
          // Clear the screen
          for (u8_t i = 0; i < m_columns*m_lines; i++)
            m_display[i] = (m_color << 8) + ' ';
          m_x = 0;
          m_y = 0;
          m_esc = 0;
          return;
        }
        break;
      case 'm':
        switch (m_escArg)
        {
          case '0':
            // Normal text
            m_color = LightGrey;
            m_esc = 0;
            return;
          case '1':
            // Bright text
            m_color = White;
            m_esc = 0;
          case '2':
            // Reversed
            m_color = (LightGrey << 4);
            m_esc = 0;
            return;
        }
    }
  } else if (m_esc == 4)
  {
    if (c == 'm' && m_escArg == '3')
    {
      // Foreground color
      m_color = (m_color & 0xf0) | m_newColor;
      m_esc = 0;
      return;
    } else if (c == 'm' && m_escArg == '4')
    {
      // Background color
      m_color = (m_color & 0x0f) | (m_newColor << 4);
      m_esc = 0;
      return;
    }
  }

  switch (c)
  {
    case '\e':
      m_esc = 1;
      return;
    case '\r':
      m_x = 0;
      break;
    case '\n':
      m_x = 0;
      m_y ++;
      break;
    case '\t':
      m_x += 8 - (m_x % 8);
      break;
    case '\b':
      m_x --;
    default:
      m_display[m_y*80 + m_x] = c | (m_color << 8);
      m_x ++;
      break;
  }

  m_esc = 0;

  if (m_x >= m_columns)
  {
    // Go to new line
    m_x = 0;
    m_y ++;
  }

  scroll(); // if needed
  moveCursor();
}

PROTECTED void VgaConsole::scroll()
{
  u8_t blank;

  /* Blank = space with color */
  blank = ' ' | (m_color << 8);

  if (m_y >= m_lines)
  {
    /* Move the current text chunk that makes up the screen back by a line */
    for (u8_t i = m_columns; i < m_columns * m_lines; i++)
      m_display[i - m_columns] = m_display[i];
    for (u8_t i = 0; i < m_color; i++)
      m_display[m_columns * (m_lines - 1) + i] = blank;
    m_y --;
  }
}

PROTECTED void VgaConsole::moveCursor()
{
  u8_t temp;

  temp = m_y * 80 + m_x;

  /* This sends a command to indices 14 and 15 in the CRT Control Register of the
   * VGA controller. These are the high and low bytes of the index that shows
   * where the hardware cursor should be blinking.
   */
  PortIO::out8(0x3d4, 14);
  PortIO::out8(0x3d5, temp >> 8);
  PortIO::out8(0x3d4, 15);
  PortIO::out8(0x3d5, temp);
}

/**
 * Clear the screen
 */
PUBLIC void VgaConsole::clear()
{
  u8_t blank;

  blank = ' ' | (m_color << 8);

  for (int i = 0; i < m_lines*m_columns; i++)
    m_display[i] = blank;

  m_x = 0; m_y = 0;
  moveCursor();
}

PUBLIC virtual word_t VgaConsole::attributes() const
{
  return OUT | DIRECT;
}

PUBLIC virtual int VgaConsole::write(const char * string, size_t length)
{
  for (size_t i = 0; i < length; i++)
    putchar(string[i]);
  return 1;
}

PUBLIC void VgaConsole::setColor(u8_t color)
{
  m_color = color;
};
