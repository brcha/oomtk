#ifndef __KERNEL_OUTPUT_STREAM_H__
#define __KERNEL_OUTPUT_STREAM_H__
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
/** @file
 * @brief Kernel (I/)O Stream
 */
#include <ostream>
#include <stdio.h>
#include <stdlib.h>

template<typename T>
class KernelOutputStream : public std::basic_ostream<T>
{
  public:
    KernelOutputStream(const char * color)
      : std::basic_ostream<T>()
    {
      _isActive = true;
      strcpy(_color, color);
    };

    virtual KernelOutputStream<T> & operator << (const T *s)
    {
      if (_isActive)
        printf("%s%s", _color, s);
      return *this;
    };

    virtual KernelOutputStream<T> & operator << (const T c)
    {
      if (_isActive)
        printf("%s%c", _color, c);
      return *this;
    };

    inline bool isActive()
    {
      return _isActive;
    };

    inline void deactivate()
    {
      _isActive = false;
    };

    inline void activate()
    {
      _isActive = true;
    };

  private:
    bool _isActive;
    char _color[10];
};


#endif /* __KERNEL_OUTPUT_STREAM_H__ */