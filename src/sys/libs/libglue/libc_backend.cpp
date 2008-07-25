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
 * \brief Glue for the LibC
 */

#include <libc_backend.h>
#include <OOMTK/OConsole>

int __libc_backend_outs(const char *s, size_t len)
{
  if (OConsole::stdout)
    return OConsole::stdout->write(s,len);
  else
    return -1;
}

int __libc_backend_ins(char *s, size_t len)
{
  if (OConsole::stdin)
  {
    size_t i;
    for(i = 0; i < len; i++)
    {
      s[i] = OConsole::stdin->getchar();
      if (s[i] == '\r')
      {
        i ++;
        break;
      }
    }
    return i;
  } else
    return 0;
}

