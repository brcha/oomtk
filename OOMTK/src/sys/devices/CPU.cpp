INTERFACE:
/*
 *  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/*
 * $Id: CPU.cpp 196 2006-10-23 13:29:52Z brcha $
 */
/** @file
 * @brief CPU Interface Methods
 */

#include "types.h"

/**
 * CPU-specific methods
 *
 * NOTE: This class must have arch-specific implementations
 */
class CPU
{
  public:
    /**
     * Disable interrupts
     */
    static void cli();

    /**
     * Enable interrupts
     */
    static void sti();

    /**
     * Are interrupts enabled?
     * @returns 1 if interrupts are enabled, 0 if not.
     */
    static word_t interrupts();

    /**
     * Disable interrupts if enabled and return their previous state.
     * @returns 1 if interrupts are enabled, 0 if not.
     */
    static word_t cliAndSave();

    /**
     * Enable interrupts if they were enabled before last @c cliAndSave.
     * @see cliAndSave()
     */
    static void stiRestore(word_t state);

    /**
     * Insert the pause instruction (nop).
     */
    static void pause();

    /**
     * Halt the processor
     *
     * Call this if you want to die.
     */
    static void halt();

    /**
     * Make a chance for kernel preemption by IRQ (several nop's)
     * @see preemptionPoint()
     */
    static void makeIrqChance();

    /**
     * Set the stack pointer
     */
    static void stackPointer(word_t sp);

    /**
     * Get the program counter
     * @returns current PC
     */
    static word_t programCounter();

    /**
     * Explicit kernel preemption point
     */
    static inline void preemptionPoint()
    {
      sti();            // Enable interrupts
      makeIrqChance();  // Wait for external IRQ
      cli();            // Disable interrupts and continue your work
    }
};
