#ifndef __ANSI_H__
#define __ANSI_H__
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** @file
 * @brief ANSI Strings for coloring the output
 */

/**
 * @defgroup ANSI
 * @brief These are some defines that make using these limited ANSI macros easier
 * @{
 */
#define ANSI_NORMAL       "\e[0m"
#define ANSI_BRIGHT       "\e[1m"
#define ANSI_REVERSED     "\e[2m"
#define ANSI_CLEAR        "\e[2J"
#define ANSI_FG_BLACK     "\e[30m"
#define ANSI_FG_RED       "\e[31m"
#define ANSI_FG_GREEN     "\e[32m"
#define ANSI_FG_YELLOW    "\e[33m"
#define ANSI_FG_BLUE      "\e[34m"
#define ANSI_FG_MAGENTA   "\e[35m"
#define ANSI_FG_CYAN      "\e[36m"
#define ANSI_FG_WHITE     "\e[37m"
#define ANSI_BG_BLACK     "\e[40m"
#define ANSI_BG_RED       "\e[41m"
#define ANSI_BG_GREEN     "\e[42m"
#define ANSI_BG_YELLOW    "\e[43m"
#define ANSI_BG_BLUE      "\e[44m"
#define ANSI_BG_MAGENTA   "\e[45m"
#define ANSI_BG_CYAN      "\e[46m"
#define ANSI_BG_GREY      "\e[47m"
/** @} */


#endif /* __ANSI_H__ */
