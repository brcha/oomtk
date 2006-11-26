/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/*
 * $Id: $
 */
/** @file
 * @brief Glue for the LibC
 */

#include <libc_backend.h>
#include <Console.h>

int __libc_backend_outs(const char *s, size_t len)
{
  if (Console::stdout)
    return Console::stdout->write(s,len);
  else
    return -1;
}

int __libc_backend_ins(char *s, size_t len)
{
  if (Console::stdin)
  {
    size_t i;
    for(i = 0; i < len; i++)
    {
      s[i] = Console::stdin->getchar();
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

