INTERFACE:
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** @file
 * @brief Paging for PAE enabled processors
 */

#include <Paging.h>

/**
 * @brief PAE Paging support for IA32
 */
class PagingPAE : public Paging
{
};

IMPLEMENTATION:

/**
 * @brief Construct the PAE Paging
 */
PUBLIC PagingPAE::PagingPAE() :
    Paging()
{
};

/**
 * @brief Initialize PAE Paging
 * @returns true if initialized correctly
 */
PUBLIC bool PagingPAE::initialize()
{
  return false;
};