/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/*
 * $Id: linker.ia32.ld 196 2006-10-23 13:29:52Z brcha $
 */
/** @file
 * @brief Linker script
 */
ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
  . = 0xF0100000;

  .text : AT(ADDR(.text) - 0xF0000000)
  {
    *(.text)
    *(.rodata*)

    /* Constructors */
    __ctors__ = .;
    *(SORT(.ctor*))
    QUAD(0)

    __dtors__ = .;
    *(SORT(.dtor*))
    QUAD(0)
  }

  .data ALIGN (0x1000) : AT(ADDR(.data) - 0xF0000000)
  {
    *(.data)
  }

  .bss : AT(ADDR(.bss) - 0xF0000000)
  {
    _sbss = .;
    *(COMMON)
    *(.bss)
    _ebss = .;
  }

  /DISCARD/ :
  {
    *(.stab*)
    *(.debug*)
    *(.comment*)
    *(.shstrtab*)
    *(.symtab*)
    *(.strtab*)
    *(.rel.*)
  }
}
