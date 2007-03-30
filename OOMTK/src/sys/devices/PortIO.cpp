INTERFACE:
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file
 * \brief Port I/O API
 *
 * NOTE: This file must have architecture specific implementations!
 */

#include <types.h>

/**
 * Ports input/output API
 */
class PortIO
{
  public:
    /**
     * Delay for slow i/o operations
     */
    static void iodelay();

    /**
     * @defgroup Normal
     * \brief Regular port i/o operations
     * @{
     */
    /**
     * Read from 8bit port
     */
    static u8_t in8(word_t port);

    /**
     * Read from 16bit port
     */
    static u16_t in16(word_t port);

    /**
     * Read from 32bit port
     */
    static u32_t in32(word_t port);

    /**
     * Write to 8bit port
     */
    static void out8(word_t port, u8_t value);

    /**
     * Write to 16bit port
     */
    static void out16(word_t port, u16_t value);

    /**
     * Write to 32bit port
     */
    static void out32(word_t port, u32_t value);
    /** @} */

    /**
     * \defgroup Delayed
     * \brief Delayed port i/o operations
     * @{
     */
    /**
     * Read from 8bit port
     */
    static u8_t in8_d(word_t port);

    /**
     * Read from 16bit port
     */
    static u16_t in16_d(word_t port);

    /**
     * Read from 32bit port
     */
    static u32_t in32_d(word_t port);

    /**
     * Write to 8bit port
     */
    static void out8_d(word_t port, u8_t value);

    /**
     * Write to 16bit port
     */
    static void out16_d(word_t port, u16_t value);

    /**
     * Write to 32bit port
     */
    static void out32_d(word_t port, u32_t value);
    /** @} */
};

IMPLEMENTATION:

// Only the delayed versions are implemented here
IMPLEMENT inline u8_t PortIO::in8_d(word_t port)
{
  u8_t tmp = in8(port);
  iodelay();
  return tmp;
}

IMPLEMENT inline u16_t PortIO::in16_d(word_t port)
{
  u16_t tmp = in16(port);
  iodelay();
  return tmp;
}

IMPLEMENT inline u32_t PortIO::in32_d(word_t port)
{
  u32_t tmp = in32(port);
  iodelay();
  return tmp;
}

IMPLEMENT inline void PortIO::out8_d(word_t port, u8_t value)
{
  out8(port,value);
  iodelay();
}

IMPLEMENT inline void PortIO::out16_d(word_t port, u16_t value)
{
  out16(port,value);
  iodelay();
}

IMPLEMENT inline void PortIO::out32_d(word_t port, u32_t value)
{
  out32(port,value);
  iodelay();
}
