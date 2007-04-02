/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file
 * \brief Abstract Port I/O API
 *
 * NOTE: This class must have architecture specific implementations!
 */
/*
 * $Id$
 */

#include "PortIO.h"

// Only the delayed versions are implemented here
static u8_t PortIO::in8_d(word_t port)
{
  u8_t tmp = in8(port);
  iodelay();
  return tmp;
}

static u16_t PortIO::in16_d(word_t port)
{
  u16_t tmp = in16(port);
  iodelay();
  return tmp;
}

static u32_t PortIO::in32_d(word_t port)
{
  u32_t tmp = in32(port);
  iodelay();
  return tmp;
}

static void PortIO::out8_d(word_t port, u8_t value)
{
  out8(port,value);
  iodelay();
}

static void PortIO::out16_d(word_t port, u16_t value)
{
  out16(port,value);
  iodelay();
}

static void PortIO::out32_d(word_t port, u32_t value)
{
  out32(port,value);
  iodelay();
}
