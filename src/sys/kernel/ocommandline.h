#ifndef __OOMTKSYS_OCOMMANDLINE_H__
#define __OOMTKSYS_OCOMMANDLINE_H__
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
/** @file ocommandline.h
 * @brief Multiboot command line
 */

#include <types.h>
#include <cstddef>

/**
 * @brief Multiboot command line
 */
class OCommandLine
{
  public:
    static OCommandLine * instance();
    
    /**
     * @brief Copy command line from physical address
     * @param commandLine physical address of the command line (possibly in the multiboot_info_t structure
     */
    void set(kpa_t commandLine);
    const char * get();
    
    /**
     * @brief Process any command line options
     * 
     * Pick off any command line options that we care about so that we
     * can release the hold on the memory that command line occupies.
     */
    void processOptions();
    
    /**
     * @brief Return argv[0] element of the command line
     * @returns the name of the kernel binary
     */
    const char * argv0();
    
    /**
     * @brief Check if command line has the option
     * @param option to be checked for presence
     * @returns true if option is found
     */
    bool hasOption (const char * option);
    
    /**
     * @brief Get the option argument if present
     * @param option whose argument is seeked
     * @returns the argument or NULL
     */
    const char * optionArg(const char * option);
    
    /**
     * @brief Check if the @p option has @p value
     * @returns true if option exists and has value equal to @p value
     */
    bool optionIsString(const char * option, const char * value);
    
    /**
     * @brief Find the integer value of the option
     * @param option whose value is seeked
     * @returns the value or 0 if not found.
     */
    unsigned long optionValue(const char * option);
    
    enum {
      CommandLineMaxSize  = 1024,
    };
    
  protected:
    OCommandLine();
    ~OCommandLine();
    
    /**
     * @brief Replace all spaces (from isspace()) with NULs to facilitate option parsing
     */
    void parseSpace();
    
    /**
     * @brief Find the named option if present
     * @param option to be found
     * @returns the option or NULL if not found
     */
    const char * findOption(const char * option);
    
  protected:
    char    m_commandLine[CommandLineMaxSize];
    bool    m_parsed;
    size_t  m_origLength;

};

#endif /* __OOMTKSYS_OCOMMANDLINE_H__ */
