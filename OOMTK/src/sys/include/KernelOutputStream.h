#ifndef __KERNEL_OUTPUT_STREAM_H__
#define __KERNEL_OUTPUT_STREAM_H__
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
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