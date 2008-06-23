/*
 *  Copyright (C) 2008 by Filip Brcic <brcha@gna.org>
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
/** \file oomtkversion.h
 * \brief This file contains the macros and functions related to the version of OOMTK.
 */
#ifndef __OOMTK_VERSION_H__
#define __OOMTK_VERSION_H__

/// \brief Version of OOMTK as a string, as defined at compile time
#cmakedefine OOMTK_VERSION_STRING "@OOMTK_VERSION_STRING@"

/// \brief Major version of OOMTK, as defined at compile time
#cmakedefine OOMTK_VERSION_MAJOR @OOMTK_VERSION_MAJOR@
/// \brief Minor version of OOMTK, as defined at compile time
#cmakedefine OOMTK_VERSION_MINOR @OOMTK_VERSION_MINOR@
/// \brief Release version of OOMTK, as defined at compile time
#cmakedefine OOMTK_VERSION_RELEASE @OOMTK_VERSION_RELEASE@

/**
 * \brief Make a number from the major, minor and release number of the OOMTK version
 */
#define OOMTK_MAKE_VERSION( a, b, c ) (((a) << 16) | ((b) << 8) | (c))

/// \brief Number form of the OOMTK compile time version.
#define OOMTK_VERSION OOMTK_MAKE_VERSION(OOMTK_VERSION_MAJOR,OOMTK_VERSION_MINOR,OOMTK_VERSION_RELEASE)

/**
 * \brief Check if the OOMTK version is at least as required
 *
 * This macro can ensure that the version of OOMTK is greater or equal to the
 * required version. The typical usage can be described by this code snippet:
 * \code
 * #if OOMTK_IS_VERSION(0,99,10)
 * // code for oomtk 0.99.xy
 * #else
 * // code for oomtk 0.80.xy
 * #endif
 * \endcode
 */
#define OOMTK_IS_VERSION(a,b,c) ( OOMTK_VERSION >= OOMTK_MAKE_VERSION(a,b,c) )

/**
 * Namespace for general OOMTK functions.
 */
namespace OOMTK
{
/**
 * \brief Returns the encoded number of OOMTK's version.
 *
 * It has the same functonality as the OOMTK_VERSION macro with the difference
 * that this function returns the runtime version, not the compile time version.
 *
 * \returns the version number, encoded as a single unsigned int.
 */
OOMTKSYS_EXPORT unsigned int version();

/**
 * \brief Returns the major version of OOMTK
 * \returns the major version number
 */
OOMTKSYS_EXPORT unsigned int versionMajor();

/**
 * \brief Returns the minor version of OOMTK
 * \returns the minor version number
 */
OOMTKSYS_EXPORT unsigned int versionMinor();

/**
 * \brief Returns the release of OOMTK
 * \returns the release number
 */
OOMTKSYS_EXPORT unsigned int versionRelease();

/**
 * \brief Returns the OOMTK version as a string
 * \returns the OOMTK version as the string.
 */
OOMTKSYS_EXPORT const char * versionString();

}

#endif /* __OOMTK_VERSION_H__ */
