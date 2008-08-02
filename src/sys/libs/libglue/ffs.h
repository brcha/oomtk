#ifndef __libglue_FFS_H__
#define __libglue_FFS_H__
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
/** @file ffs.h
 * @brief Find first bit set in a word
 */

#include <macros.h>

__BEGIN_DECLS

/**
 * @brief Find first bit set in @p val
 * @returns the position of the first bit set, or 0 if no bits are set in @p val
 */
int ffs(int val);
/**
 * @brief Find first bit set in @p val
 * @returns the position of the first bit set, or 0 if no bits are set in @p val
 */
int ffsl(long val);
/**
 * @brief Find first bit set in @p val
 * @return the position of the first bit set, or 0 if no bits are set in @p val
 */
int ffsll(long long val);

__END_DECLS

#endif /* __libglue_FFS_H__ */
