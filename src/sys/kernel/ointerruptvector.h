#ifndef __OOMTKSYS_OINTERRUPTVECTOR_H__
#define __OOMTKSYS_OINTERRUPTVECTOR_H__
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
/** @file ointerruptvector.h
 * @brief Architecture-independant interrupt vector class
 */

#include <types.h>
#include <cstddef>
#include <macros.h>

#include INC_ARCH(irq.h)
#include INC_ARCH(registers.h)

#include <OOMTK/OIRQController>
#include <OOMTK/OStallQueue>
#include <OOMTK/OSpinLock>

class OProcess;
class OVectorHandlers;

/**
 * @brief Interrupt vector class
 */
class OInterruptVector
{
  public:
    /**
     * @brief Initialize the vector table and corresponding IDT entries
     *
     * @warning This method has architecture dependant implementation
     * @note This is the closest thing to a constructor for this class, but it doesn't construct one
     * instance, instead it constructs the whole array of NUM_VECTORS size.
     */
    static void initialize();

  protected:
    /// @brief Constructor is not meant to be used by mortals
    OInterruptVector();
    ~OInterruptVector();

  public:

    /**
     * @brief Interrupt vector handler function type
     *
     * @note This could have been done by inheriting OInterruptVector class for each and
     * every one of the interrupts. But that has two disadvantages. First, it would make
     * 256 inherited classes only to inherit one method. Second, the interrupts are held
     * in a static array because they need to be initialized before memory allocation and
     * other fancy things like that. So no inheritance is allowed and hence interrupt handlers
     * should be made elsewhere and put into object of this class as pointers.
     *
     * @param self this is a @em this pointer, named as @em self because @em this is a reserved
     * word and @em self is commonly used in python (and possibly other oo languages) in place
     * of @em this.
     * @param process process that is the source of this interrupt
     * @param saveArea registers saved on context switch to this handler
     */
    typedef void (*handler_t) (OInterruptVector * self, OProcess * process, fixed_regs_t * saveArea);

    /**
     * @brief Interrupt vector types
     *
     * Values for OInterruptVector::m_type field.
     */
    enum VectorType
    {
      vt_Unbound,               ///< @brief Not yet bound
      vt_HardwareTrap,          ///< @brief Hardware trap or exception
      vt_SystemCall,            ///< @brief System call (software trap)
      vt_Interrupt,             ///< @brief Interrupt
    };

    /**
     * @brief Interrupt vector modes
     *
     * Values for OInterruptVector::m_mode field.
     */
    enum VectorMode
    {
      vm_FromBUS,               ///< @brief BUS defined vector mode
      vm_Edge,                  ///< @brief Edge activated
      vm_Level,                 ///< @brief Level activated
    };

    /**
     * @brief Interrupt vector levels
     *
     * Values for OInterruptVector::m_level field.
     */
    enum VectorLevel
    {
      vl_FromBUS,               ///< @brief BUS defined vector level
      vl_ActiveHigh,            ///< @brief Active on high level/rising edge
      vl_ActiveLow,             ///< @brief Active on low level/dropping edge
    };

    /**
     * @brief Interrupt vector status
     *
     * Values for OInterruptVector::m_status field.
     */
    enum VectorStatus
    {
      vs_Uninitialized,         ///< @brief Nothing is known about this vector
      vs_Live,                  ///< @brief Interrupts/events on this vector will be delivered
      vs_Fired,                 /**< @brief Int/event detected, but not yet delivered
                                 *
                                 * This means that the interrupt has been detected, but no application
                                 * (or user-mode driver/handler) has shown interest about it. Therefore,
                                 * the vector is masked in the hardware and acknoledged if needed.
                                 */
      vs_Delivered,             /**< @brief Int/event has been delivered
                                 *
                                 * This means that the user-mode handler has been given the interrupt info,
                                 * but it hasn't shown interest in receiving the next interrupt from this
                                 * vector. Probably the handler is handling the interrupt right now.
                                 */
    };

  struct vector_hold_info_t : public OSpinLock::hold_info_t
    {
      vector_hold_info_t(OSpinLock::hold_info_t hi) : OSpinLock::hold_info_t(hi) {};
      flags_t oldFlags;
    };

  public:
    /**
     * @brief Lock a vector entry for manipulation, including guarding against interrupts
     * @returns vector hold info for releaseing the lock
     */
    vector_hold_info_t grab();

    /**
     * @brief Release a vector entry and allow further interrupts
     * @param hi vector hold info from grab()
     */
    void release (vector_hold_info_t hi);

    /**
     * @brief Set the status of this vector
     * @param status new status
     */
    void setStatus (VectorStatus status);

    /**
     * @brief Bind an interrupt to a handler
     * @param irq interrupt identifier
     * @param mode interrupt mode
     * @param level interrupt level
     * @param handler handler function
     *
     * @warning This method has architecture dependant implementation
     */
    static void irqBind (irq_t irq, VectorMode mode, VectorLevel level, handler_t handler);

    /**
     * @brief Return the interrupt vector entry mapped to this irq
     * @param irq interrupt identifier
     * @returns interrupt vector entry mapped to @p irq
     *
     * @warning This method has architecture dependant implementation
     */
    static OInterruptVector * mapInterrupt (irq_t irq);
    
    /**
     * @brief Set hardware interrupt vector entry
     * @param entry interrupt vector entry
     * @param handlerPtr interrupt handler method pointer
     * @param allowUser if user-mode interrupts are allowed
     * 
     * @warning This method has architecture dependant implementation
     */
    static void setHardwareVector(int entry, void (*handlerPtr)(void), bool allowUser);
    
    /**
     * @brief Global interrupt/trap/exception handler
     * @param inProc Process in which the interrupt occured (or NULL if in kernel)
     * @param saveArea Register set saved on context switch
     * 
     * @note This function will not always return. It returns if the interrupt was in kernel
     * @note This function is called from arch/ia32/interrupts.S
     */
    static void OnTrapOrInterrupt(OProcess * inProc, fixed_regs_t * saveArea) asm ("irq_OnTrapOrInterrupt");

  protected:
    handler_t       m_handler;    ///< @brief Handler function
    uint64_t        m_count;      ///< @brief Number of occurrences (hopefully less than 2^64 :) )
    uint8_t         m_type;       ///< @brief Vector type, @see VectorType
    bool            m_user;       ///< @brief User accessible
    uint8_t         m_mode : 2;   ///< @brief Trigger mode (@see VectorMode)
    uint8_t         m_level : 2;  ///< @brief Active on high/low level (@see VectorLevel)
    uint8_t         m_status : 2; ///< @brief Current vector state (@see VectorStatus)
    uint32_t        m_irq;        ///< @brief Global interrupt pin number
    OIRQController* m_controller; ///< @brief Interrupt controller chip

    OStallQueue     m_stallQ;     ///< @brief Queue processes on this vector entry

    OInterruptVector  * m_next;

    /// @brief Interrupt vectors
    static OInterruptVector   m_vectorMap[NUM_VECTOR];
    /// @brief Reverse map from IRQs to vector entries
    static OInterruptVector * m_irqVector[NUM_IRQ];
    /// @brief Number of interrupt sources
    static irq_t              m_numGlobalIRQ;
    
    friend class OVectorHandlers;
};

#endif /* __OOMTKSYS_OINTERRUPTVECTOR_H__ */
