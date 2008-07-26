/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
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
#include <macros.h>
#include <ascii.h>

#include "ovgaconsole.h"

#include <cstring>
#include <cctype>
#include <assert.h>

#include "oportio.h"

OVgaConsole * OVgaConsole::instance()
{
  static OVgaConsole _instance = OVgaConsole();

  return &_instance;
};

/**
 * \brief Protected constructor -- not for use by mortals
 */
OVgaConsole::OVgaConsole()
{
  m_display = PTOKV(0xb8000, uint16_t *);
  m_color = WhiteOnBlack;
  m_newColor = 0;
  m_esc = 0;
  m_escArg = 0;
  m_columns = 80;
  m_lines = 25;
  m_scrollMode = Emacs;
}

void OVgaConsole::initialize()
{
  uint8_t hi, lo; // high and low start address registers */
  
  OPortIO::out8_d(0x3d4, 0xc);    // get high start address register
  hi = OPortIO::in8_d(0x3d5);
  
  OPortIO::out8_d(0x3d4, 0xd);    // get low start address register
  lo = OPortIO::in8_d(0x3d5);
  
  m_startAddressRegister = ((uint32_t) hi) << 8 | lo;
  m_offset = 0;
  
  clear();
  showCursorAt(0);
  state(OUTPUT);
}

void OVgaConsole::setAt(uint32_t pos, char c)
{
  m_display[pos] = ((m_color) << 8) | c;
}

void OVgaConsole::clear()
{
  uint32_t oldColor = m_color;
  m_color = WhiteOnBlack;
  for (uint32_t pos = 0; pos < (m_lines * m_columns); pos++)
    setAt(pos, ' ');
  m_color = oldColor;
}

void OVgaConsole::scroll(uint32_t start, uint32_t end, int amount)
{
  uint32_t pos = 0;
  
  if (amount > 0)
  {
    uint32_t gap = amount;
    
    for (pos = start + gap; pos < end; pos++)
      m_display[pos] = m_display[pos - gap];
    
    for (pos = start; pos < start + gap; pos++)
      m_display[pos] = (WhiteOnBlack << 8);
  } else
  {
    uint32_t gap = -amount;
    
    for (pos = start; pos < end - gap; pos++)
      m_display[pos] = m_display[pos + gap];
    
    for (pos = end - gap; pos < end; pos++)
      m_display[pos] = (WhiteOnBlack << 8);
  }
}

void OVgaConsole::showCursorAt(uint32_t pos)
{
  uint32_t cursorAddress = (uint32_t) pos;
  
  cursorAddress += m_startAddressRegister;
  
  OPortIO::out8_d(0x3d4, 0xe);
  OPortIO::out8_d(0x3d5, (cursorAddress >> 8) & 0xffu);
  OPortIO::out8_d(0x3d4, 0xf);
  OPortIO::out8_d(0x3d5, (cursorAddress & 0xffu));
}

void OVgaConsole::putchar(char c)
{
  // Check if the console has active output
  if (! (state() & OUTPUT))
    return;
  
  uint32_t posColumn = m_offset % m_columns;
  
  static const uint8_t colors[] = {0, 4, 2, 14, 1, 5, 3, 15};

  if (m_esc == 1)
  {
    if (c == '[')
    {
      m_esc ++;
      return;
    } else
      m_esc = 0; // reset the escape state... (maybe print the escape somehow?)
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
          clear();
          m_esc = 0;
          return;
        }
        break;
      case 'm':
        switch (m_escArg)
        {
          case '0':
            // Normal text
            m_color = WhiteOnBlack;
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

  if (c == ASCII_CR)
    if (m_offset % m_columns)
      scroll(m_offset, m_offset + (m_columns - posColumn), m_columns - posColumn);
  
  if (isprint(c))
  {
    setAt(m_offset, c);
    m_offset ++;
  } else
  {
    switch (c)
    {
      case ASCII_BEL:
	beep();
	break;
      case ASCII_BS:
	// Backspace
	if (m_offset % m_columns)
	{
	  m_offset--;
	  setAt(m_offset, ' ');
	}
	break;
      case ASCII_TAB:
	// Reach the first position that is divideable by 8 (tabstop)
	while (m_offset % 8)
	{
	  setAt(m_offset, ' ');
	  m_offset ++;
	}
      case ASCII_LF:
	// Linefeed
	m_offset += m_columns;
	m_offset -= (m_offset % m_columns);
	break;
      case ASCII_VT:
	// Reverse linefeed
	if (m_offset > m_columns)
	  m_offset -= m_columns;
	break;
      case ASCII_CR:
	m_offset -= (m_offset % m_columns);
	break;
      case ASCII_ESC:
	m_esc = 1;
	return;
    }
  }

  m_esc = 0;

  if (m_offset >= m_lines * m_columns)
  {
    if (m_scrollMode == Emacs)
    {
      // Scroll lower third of the screen
      scroll(0, m_lines * m_columns, - (int) (m_lines / 3) * m_columns);
      m_offset -= (m_lines / 3) * m_columns;
    } else
    {
      scroll(0, m_lines * m_columns, - (int) m_columns);
      m_offset -= m_columns;
    }
  }
  
  assert(m_offset < m_lines * m_columns);
  showCursorAt(m_offset);
}

word_t OVgaConsole::attributes() const
{
  return OUT | DIRECT;
}

int OVgaConsole::write(const char * string, size_t length)
{
  for (size_t i = 0; i < length; i++)
    putchar(string[i]);
  return 1;
}

void OVgaConsole::setColor(uint8_t color)
{
  m_color = color;
};

void OVgaConsole::setScrollMode(uint8_t mode)
{
  m_scrollMode = mode;
}