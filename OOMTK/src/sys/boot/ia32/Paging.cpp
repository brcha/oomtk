INTERFACE:
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** @file
 * @brief Paging support
 *
 * @todo Is this file IA32 specific or generic???
 */

/**
 * @brief Paging base class
 *
 * This is the base class for paging support. It has to be inherited and implemented
 * for specific architecture and specific paging method.
 */
class Paging
{
};

IMPLEMENTATION:

PUBLIC static Paging * Paging::instance()
{
  static Paging paging = Paging();

  return &paging;
};