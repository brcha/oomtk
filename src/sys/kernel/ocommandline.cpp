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
#include "ocommandline.h"

#include <cstring>
#include <cstddef>
#include <cctype>
#include <cstdlib>
#include <fatal.h>
#include <vpstring.h>

OCommandLine::OCommandLine() :
    m_parsed(false), m_origLength(0)
{
}


OCommandLine::~OCommandLine()
{
}

void OCommandLine::parseSpace()
{
  if (m_parsed)
    return;
  
  m_parsed = true;
  
  m_origLength = strlen(m_commandLine);
  
  for (size_t i = 0; i < m_origLength; i++)
    if (isspace(m_commandLine[i]))
      m_commandLine[i] = 0;
}

const char * OCommandLine::argv0()
{
  parseSpace();
  
  for (size_t i = 0; i < m_origLength; i++)
    if (m_commandLine[i])
      return &m_commandLine[i];
  
  fatal ("Command line had no content!\n");
}

const char * OCommandLine::findOption(const char * option)
{
  parseSpace();
  
  size_t len = strlen(option);
  
  for (size_t i = 0; i < m_origLength; i++)
  {
    if (m_commandLine[i] != *option)
      continue;
    
    if (memcmp(&m_commandLine[i], option, len) != 0)
      continue;
    
    // The option can be terminated with '=' or NUL
    if (m_commandLine[i+len] && m_commandLine[i + len] != '=')
      continue;
    
    return &m_commandLine[i];
  }
  
  return NULL;
}

bool OCommandLine::hasOption(const char * option)
{
  return findOption(option) != NULL;
}

const char * OCommandLine::optionArg(const char * option)
{
  const char * s = findOption(option);
  
  if (s == NULL)
    return NULL; // there is no option
  
  while (*s && *s != '=')
    s++;
  
  if (! *s)
    return NULL; // there is no argument
  
  return s;
}

bool OCommandLine::optionIsString(const char * option, const char * value)
{
  const char * s = optionArg(option);
  return (s && (strcmp(s, value) == 0));
}

unsigned long OCommandLine::optionValue(const char * option)
{
  const char * s = optionArg(option);
  
  if (!s)
    return 0;
  
  return strtoul(s, 0, 0);
}

void OCommandLine::processOptions()
{
  // no command line options at the moment...
}

void OCommandLine::set(kpa_t commandLine)
{
  // Multiboot command line is null terminated, but the length is unknown. Therefore
  // we copy only CommandLineMaxSize and NUL terminate by hand.
  // Memory allocation is not available yet, so ... :)
  memcpy_ptov(m_commandLine, commandLine, CommandLineMaxSize);
  m_commandLine[CommandLineMaxSize - 1] = 0;
}

const char * OCommandLine::get()
{
  return m_commandLine;
}

OCommandLine * OCommandLine::instance()
{
  static OCommandLine cmdLine = OCommandLine();
  
  return &cmdLine;
}



