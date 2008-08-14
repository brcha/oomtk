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
#include <OOMTK/OInterruptVector>
#include <OOMTK/OCPU>
#include <OOMTK/OProcess>
#include <OOMTK/OReadyQueue>

#include <fatal.h>
#include <cassert>
#include <macros.h>
#include <cstdio>

#include <ia32/EFLAGS.h>
#include <ia32/selectors.h>

#define DEBUG_USER_PAGEFAULT  DEBUG_ENABLE
#define DEBUG_PAGEFAULT       DEBUG_ENABLE

void irq_DoTripleFault() NORETURN;

/**
 * @brief Interrupt vectors
 */
enum Vectors
{
  vec_DivZero             = 0x00,
  vec_Debug               = 0x01,
  vec_NMI                 = 0x02,
  vec_BreakPoint          = 0x03,
  vec_Overflow            = 0x04,
  vec_Bounds              = 0x05,
  vec_BadOpcode           = 0x06,
  vec_DeviceNotAvail      = 0x07,
  vec_DoubleFault         = 0x08,
  vec_CoprocessorOverrun  = 0x09,
  vec_InvalidTSS          = 0x0a,
  vec_SegNotPresent       = 0x0b,
  vec_StackSeg            = 0x0c,
  vec_GeneralProtection   = 0x0d,
  vec_PageFault           = 0x0e,
      // 0x0f is intel reserved
  vec_CoprocError         = 0x10,
  vec_AlignCheck          = 0x11,
  vec_MachineCheck        = 0x12,
  vec_SIMDfp              = 0x13,
      // 0x14..0x1f are intel reserved
      
  vec_IRQ0                = NUM_TRAP,
      
      // Following vectors are free for use, but must not collide with hardware interrupts
  vec_Syscall             = 0x30, // Thought about 0x80, but that collides with linux...
};
    
/**
 * @brief Hardware interrupt busses and pins known to the kernel
 */
enum Interrupts
{
  irq_ISA_PIT           = IRQ(IBUS_ISA, 0),   ///< @brief CMOS Interval Timer
  irq_ISA_Keyboard      = IRQ(IBUS_ISA, 1),   ///< @brief Keyboard
  irq_ISA_Cascade       = IRQ(IBUS_ISA, 2),   ///< @brief Master 8259 cascade from secondary
  irq_LAPIC_IPI         = IRQ(IBUS_LAPIC, 0xf8),
  irq_LAPIC_Timer       = IRQ(IBUS_LAPIC, 0xfe),
  irq_LAPIC_SVR         = IRQ(IBUS_LAPIC, 0xff),
};
    
/**
 * @brief IA32 Gate descriptor
 */
struct ia32_gate_descriptor_t
{
  uint32_t  offset_lo : 16;
  uint32_t  selector  : 16;
  uint32_t  zero      : 8;
  uint32_t  type      : 4;
  uint32_t  system    : 1;
  uint32_t  dpl       : 2;
  uint32_t  present   : 1;
  uint32_t  offset_hi : 16;
};

/**
 * @brief Hardware-level interrupt dispatch table.
 */
CACHE_ALIGN ia32_gate_descriptor_t IdtTable[NUM_VECTOR];

OInterruptVector * OInterruptVector::mapInterrupt(irq_t irq)
{
  switch (IRQ_BUS(irq))
  {
    case IBUS_ISA:
    {
      // TODO: IntSrcOverride isovr;
      // if (acpi_map_interrupt(irq, &isovr))
      //    irq = isovr.globalSystemInterrupt;
      // else
      irq = IRQ(IBUS_GLOBAL, IRQ_PIN(irq));
      return m_irqVector[irq];
    }
    case IBUS_GLOBAL:
      return m_irqVector[irq];
    case IBUS_LAPIC:
      // LAPIC interrupts are programmed with vector number directly
      return &m_vectorMap[IRQ_PIN(irq)];
    default:
      fatal("Unknown bus type for binding.\n");
  }
}

void OInterruptVector::irqBind(irq_t irq, OInterruptVector::VectorMode mode,
                               OInterruptVector::VectorLevel level, OInterruptVector::handler_t handler)
{
  OInterruptVector * vector;
  
  switch (IRQ_BUS(irq))
  {
    case IBUS_ISA:
    {
      mode = OInterruptVector::vm_Edge;
      level = OInterruptVector::vl_ActiveHigh;
      
      // TODO: IntSrcOverride isovr;
      // if (acpi_map_interrupt(irq, &isovr))
      // {
      //    irq = isovr.globalSystemInterrupt;
      //    if ((isovr.flags & MPS_INTI_TRIGGER) == MPS_INTI_TRIGGER_LEVEL)
      //        mode = OInterruptVector::vm_Level;
      //    if ((isovr.flags & MPS_INTI_POLARITY) == MPS_INTI_POLARITY_ACTIVELOW)
      //        level = OInterruptVector::vl_ActiveLow;
      // } else
      irq = IRQ(IBUS_GLOBAL, IRQ_PIN(irq));
      
      vector = m_irqVector[irq];
      break;
    }
    case IBUS_LAPIC:
    {
      vector = &m_vectorMap[IRQ_PIN(irq)];
      break;
    }
    default:
      fatal("Unknown bus type for binding.\n");
  }
  
  assert(vector);
  
  vector_hold_info_t hi = vector->grab();
  vector->m_count = 0;
  vector->m_handler = handler;
  vector->m_mode = mode;
  vector->m_level = level;
  vector->m_controller->setup(vector);
  vector->release(hi);
};

void OInterruptVector::setHardwareVector(int entry, void (*handlerPtr)(void), bool allowUser)
{
  uint32_t handlerPtrAddr = reinterpret_cast<uint32_t>(handlerPtr);

  IdtTable[entry].offset_lo = (uint16_t) handlerPtrAddr;
  IdtTable[entry].selector  = sel_KernelCode;
  IdtTable[entry].zero      = 0;
  IdtTable[entry].type      = 0xeu;
  IdtTable[entry].system    = 0;
  IdtTable[entry].dpl       = allowUser ? 3 : 1;
  IdtTable[entry].present   = 1;
  IdtTable[entry].offset_hi = (uint16_t) (handlerPtrAddr >> 16);
}

/// @brief Dump the registers on fatal fault (or on some other occasion)
void dumpSaveArea(fixed_regs_t * regs)
{
  if (((regs->EFLAGS & EFLAGS_VM) == 0) && regs->CS && ((regs->CS & 0x3u) == 0x0u))
  {
    // This is a kernel-mode trap
    uint16_t  ss;
    uint16_t  ds;
    uint16_t  es;
    uint16_t  fs;
    uint16_t  gs;
    GNU_ASM("mov %%ss, %0\n" : "=r" (ss));
    GNU_ASM("mov %%ds, %0\n" : "=r" (ds));
    GNU_ASM("mov %%es, %0\n" : "=r" (es));
    GNU_ASM("mov %%fs, %0\n" : "=r" (fs));
    GNU_ASM("mov %%gs, %0\n" : "=r" (gs));
    
    printf("CS:EIP %04x:%08x SS:ESP %04x:%08x (kern %08x)\n"
           "DS  %04x ES  %04x FS  %04x GS  %04x\n"
           "EAX %08x EBX %08x ECX %08x EDX %08x\n"
           "EDI %08x ESI %08x ESP %08x EBP %08x\n"
           "VecNo %02x Error %04x ExAddr %08x\n",
           regs->CS, regs->EIP, ss, ((uint32_t) regs) + offsetof(fixed_regs_t, ESP), regs,
           ds, es, fs, gs,
           regs->EAX, regs->EBX, regs->ECX, regs->EDX,
           regs->EDI, regs->ESI, regs->ESP, regs->EBP,
           regs->ExceptNo, regs->Error, regs->ExceptAddr);
  }
  else
  {
    // User-mode interrupt
    printf("CS:EIP %04x:%08x SS:ESP %04x:%08x (user %08x)\n"
        "DS  %04x ES  %04x FS  %04x GS  %04x\n"
        "EAX %08x EBX %08x ECX %08x EDX %08x\n"
        "EDI %08x ESI %08x ESP %08x EBP %08x\n"
        "VecNo %02x Error %04x ExAddr %08x\n",
        regs->CS, regs->EIP, regs->SS, regs->ESP, regs,
        regs->DS, regs->ES, regs->FS, regs->GS,
        regs->EAX, regs->EBX, regs->ECX, regs->EDX,
        regs->EDI, regs->ESI, regs->ESP, regs->EBP,
        regs->ExceptNo, regs->Error, regs->ExceptAddr);
  }
  
  printf("FLAGS=%s %s %s %s %s %s %s %s %s IOPL=%s %s %s %s %s %s %s %s\n",
        (regs->EFLAGS & EFLAGS_CF) ? "CF" : "cf",
        (regs->EFLAGS & EFLAGS_PF) ? "PF" : "pf",
        (regs->EFLAGS & EFLAGS_AF) ? "AF" : "af",
        (regs->EFLAGS & EFLAGS_ZF) ? "ZF" : "zf",
        (regs->EFLAGS & EFLAGS_SF) ? "SF" : "sf",
        (regs->EFLAGS & EFLAGS_TF) ? "TF" : "tf",
        (regs->EFLAGS & EFLAGS_IF) ? "IF" : "if",
        (regs->EFLAGS & EFLAGS_DF) ? "DF" : "df",
        (regs->EFLAGS & EFLAGS_OF) ? "OF" : "of",
        (regs->EFLAGS & EFLAGS_IOPL) == 0 ? "kernel" : "user",
        (regs->EFLAGS & EFLAGS_NT) ? "NT" : "nt",
        (regs->EFLAGS & EFLAGS_RF) ? "RF" : "rf",
        (regs->EFLAGS & EFLAGS_VM) ? "VM" : "vm",
        (regs->EFLAGS & EFLAGS_AC) ? "AC" : "ac",
        (regs->EFLAGS & EFLAGS_VIF) ? "VIF" : "vif",
        (regs->EFLAGS & EFLAGS_VIP) ? "VIP" : "vip",
        (regs->EFLAGS & EFLAGS_ID) ? "ID" : "id");
}

/**
 * @brief Vector handlers
 * 
 * @note This class exists solely for making it easy to make friends with other classes.
 * On various architectures there are various vector handlers and one line "friend class
 * OVectorHandlers" in OProcess, OInterruptVector (and other classes if needed) can make
 * them all friends.
 */
class OVectorHandlers
{
  public:
    static void DebugException(OInterruptVector * vec, OProcess * inProc, fixed_regs_t * saveArea)
    {
      assert (inProc);
      // TODO: proc_TakeFault(inProc, coyotos_Process_FC_Debug, 0);
    }
    
    static void BptTrap(OInterruptVector * vec, OProcess * inProc, fixed_regs_t * saveArea)
    {
      assert (inProc);
      
      inProc->m_regs.fixed.EIP -= 0x1; // Correct the EIP
      // TODO: proc_TakeFault(inProc, coyotos_Process_FC_BreakPoint, 0);
    }
    
    static void FatalFault(OInterruptVector * vec, OProcess * inProc, fixed_regs_t * saveArea)
    {
      uint32_t vecno = saveArea->ExceptNo;
      
      switch (vecno)
      {
        // Faults that are system-wide fatal:
        case vec_NMI:
          fatal ("Unexpected NMI\n");
        case vec_DoubleFault:
          fatal ("Unexpected Double Fault\n");
        case vec_InvalidTSS:
          fatal ("Invalid TSS\n");
        case vec_CoprocError:
          fatal ("Coprocessor Error\n");
        case vec_MachineCheck:
          fatal ("Machine Check\n");
      }
    }
    
    static void DeviceNotAvailable(OInterruptVector * vec, OProcess * inProc, fixed_regs_t * saveArea)
    {
      fatal ("Device not available\n");
    }
    
    enum PagefaultError
    {
      Error_P         = 0x01,   ///< @brief If clear, not-present fault
      Error_RW        = 0x02,   ///< @brief If set, write attempt
      Error_US        = 0x04,   ///< @brief If set, user mode
      Error_RSV       = 0x08,   ///< @brief If set, reserved bits not 0
      Error_ID        = 0x10,   ///< @brief If set, instruction fetch
    };
    
    static void PageFault(OInterruptVector * vec, OProcess * inProc, fixed_regs_t * saveArea)
    {
      uint32_t e = saveArea->Error;
      uint32_t addr = saveArea->ExceptAddr;
      
      DEBUG_PAGEFAULT printf("Page fault trap va=" OOMTK_KVA_FMT ", errorCode %x\n",
                             saveArea->ExceptAddr, saveArea->Error);
      
      if (e & Error_RSV)
        fatal ("Page fault due to non-zero reserved bits\n");
      
      assert ((inProc == 0) == ((e & Error_US) == 0));
      
      if (addr >= KVA && (e & Error_US) == 0)
        fatal ("Supervisor-mode pagefault at " OOMTK_KVA_FMT "\n", addr);
      
      if (inProc == 0)
      {
        // Pagefault form kernel mode while attempting to access user-mode address.
        // This is possible only from the invocation path. Attribute the pagefault
        // to the current process and proceed as the exception occured from user mode.
        inProc = OCPU::current()->m_currentProcess;
        assert (inProc);
        DEBUG_USER_PAGEFAULT
            printf ("User-address pagefault in supervisor mode at EIP=" OOMTK_KVA_FMT ", VA=" OOMTK_KVA_FMT "\n",
                    saveArea->EIP, addr);
        // Enable interrupts to allow preemption
        GNU_ASM("sti");
      }
      
      // TODO: bool wantExec = IA32_NXSupported && (e & Error_ID);
      bool wantExec = false;
      bool wantWrite = (e & Error_RW);
      
      DEBUG_PAGEFAULT
          printf ("do_pageFault(proc=%P, va=%08P, %s, %s, false)\n",
                  inProc, addr, wantWrite?"wantWrite":0, wantExec?"wantExec":0);
      
      // TODO: do_pageFault(inProc, addr, wantWrite, wantExec, false);
      
      DEBUG_PAGEFAULT printf ("do_pageFault(...) done\n");
      
      // TODO: vm_switch_curcpu_to_map(inProc->mapping);
    }
    
    static void UserFault(OInterruptVector * vec, OProcess * inProc, fixed_regs_t * saveArea)
    {
      if (inProc == 0)
      {
        printf ("User fault from kernel???:\n");
        dumpSaveArea(saveArea);
        fatal("This shouldn't have happened...\n");
      }
      
      uint32_t vecno = saveArea->ExceptNo;
      
      switch(vecno)
      {
        case vec_DivZero:
          // TODO: proc_TakeFault(inProc, coyotos_Process_FC_DivZero, 0);
        case vec_Overflow:
          // TODO: proc_TakeFault(inProc, coyotos_Process_FC_Overflow, 0);
        case vec_Bounds:
          // TODO: proc_TakeFault(inProc, coyotos_Process_FC_Bounds, 0);
        case vec_BadOpcode:
          // TODO: proc_SetFault(inProc, coyotos_Process_FC_BadOpcode, 0);
        case vec_GeneralProtection:
        {
          // If this is GP(0), and the domain holds the DevicePrivs key in a register,
          // but it's privilege level is not appropriate for I/O access, escalate it's
          // privilege level.
          
          // TODO: Check if this is needed and implement it or erase it
#if 0
          if (inProc)
          {
            uint32_t iopl = saveArea->EFLAGS & EFLAGS_IOPL;
            if (saveArea->Error == 0 &&
                iopl < EFLAGS_IOPL_RING3 &&
                inProc->m_ioSpace.type = ct_IOPriv)
            {
              // No need to mask out old value since we are writing all 1's to the field
              inProc->m_regs.fixed.EFLAGS |= EFLAGS_IOPL_RING3;
              return;
            }
          }
#endif
          printf("Took a %s general protection fault. Saved state is:\n",
                 inProc ? "process" : "kernel");
          dumpSaveArea(saveArea);
          // TODO: proc_TakeFault(inProc, coyotos_Process_FC_GeneralProtection, 0);
        }
        case vec_StackSeg:
          // TODO: proc_TakeFault(inProc, coyotos_Process_FC_StackSeg, 0);
        case vec_SegNotPresent:
          // TODO: proc_TakeFault(inProc, coyotos_Process_FC_SegNotPresent, 0);
        case vec_AlignCheck:
          // TODO: proc_TakeFault(inProc, coyotos_Process_FC_BadAlign, 0);
        case vec_SIMDfp:
          // TODO: proc_TakeFault(inProc, coyotos_Process_FC_SIMDfp, 0);
        default:
          fatal("This method is not implemented yet!\n"); // remove this line then TODOs are fixed
      }
    }
    
    static void ReservedException(OInterruptVector * vec, OProcess * inProc, fixed_regs_t * saveArea)
    {
      fatal ("Intel reserved exception %x\n", saveArea->ExceptNo);
    }
    
    static void SysCall(OInterruptVector * vec, OProcess * inProc, fixed_regs_t * saveArea)
    {
      assert (inProc);
      
      // Load the PC and SP values from their source registers first to match syscall/sysenter
      inProc->m_regs.fixed.ESP  = inProc->m_regs.fixed.ECX;
      inProc->m_regs.fixed.EIP  = inProc->m_regs.fixed.EDX;
      
      // TODO: proc_syscall();
      
      if (inProc->m_issues.get() & inProc->issue_SysCallDone)
        inProc->m_regs.fixed.EIP += 2;
    }
    
    static void UnboundIRQ(OInterruptVector * vec, OProcess * inProc, fixed_regs_t * saveArea)
    {
      fatal ("Received orphaned %s interrupt %d [err=%x]\n",
             inProc ? "process" : "kernel", vec->m_irq, saveArea->Error);
    }
    
    /**
     * @brief Handler for user-mode interrupts
     */
    static void BoundIRQ(OInterruptVector * vec, OProcess * inProc, fixed_regs_t * saveArea)
    {
      assert (!IRQ::localInterruptsEnabled());
      
      assert (vec->m_status == vec->vs_Live);
      vec->m_status = vec->vs_Fired;
      
      if (!vec->m_stallQ.isEmpty())
      {
        vec->m_next = OCPU::current()->m_wakeVectors;
        OCPU::current()->m_wakeVectors = vec;
      }
    }
    
    /**
     * @brief Handler for unbound vectors
     */
    static void UnboundVector(OInterruptVector * vec, OProcess * inProc, fixed_regs_t * saveArea)
    {
      fatal("Trap from %s to unbound vector %d [err=%x]\n",
            inProc ? "process" : "kernel", saveArea->ExceptNo, saveArea->Error);
    }
};

typedef void (*trap_stub_t)();
/**
 * @brief Stub table defined in arch/ia32/interrupts.S
 */
extern trap_stub_t irq_stubs[NUM_VECTOR];

struct descriptor_table_pointer_t
{
  uint16_t  size;
  uint32_t  lowAddr __attribute((packed));
  uint16_t  : 16; /* pad */
};

void OInterruptVector::OnTrapOrInterrupt(OProcess * inProc, fixed_regs_t * saveArea)
{
  // If we are coming in from usermode, this is either an interrupt that preepted or an exception.
  // In either case all CPU flags should be zero.
  if (inProc)
    assert (OCPU::current()->m_flags.get() == 0);
  
  OInterruptVector * vector = &m_vectorMap[saveArea->ExceptNo];
  
  // Low level hardware interrupt handlers run disabled.
  // Low level hardware interrupt handlers never change the value
  // of OCPU::current()->m_currentProcess;
  if (vector->m_type == vt_Interrupt) // hardware interrupt
  {
    OProcess * preIrqProc = OCPU::current()->m_currentProcess;
    
    irq_t irq = vector->m_irq;
    OIRQController * cntrlr = vector->m_controller;
    
    // 8259 has problems with irq7 and irq15. APIC have race conditions in some cases.
    // Rather than wake up the user mode drivers for interrupts that are instantaneously
    // deasserted, check here whether the interrupt that we received is still pending.
    // If not, simply acknowledge the PIC and return.
    if (!cntrlr->isPending(vector))
    {
      printf/*fatal*/ ("IRQ %d no longer pending\n", irq);
      cntrlr->ack(vector);
      return;
    }
    
    // Interrupt is pending. Mask the interrupt and acknowledge.
    vector->setStatus(vs_Fired);
    vector->m_count ++;
    vector->m_handler(vector, inProc, saveArea);
    
    assert (preIrqProc == OCPU::current()->m_currentProcess);
    
    if (inProc && (OCPU::current()->m_flags.get() & OCPU::flag_WasPreempted))
    {
      OReadyQueue::mainRQ()->add(inProc, false);
      
      // Enable interrupts because choosing a new process can involve some work
      GNU_ASM("sti");
      
      // TODO: sched_abandon_transaction();
    }
    // The driver should reenable the interrupt if it needs it.
  }
  else
  {
    GNU_ASM("sti");
    
//     if (inProc)
//TODO:       inProc->m_hdr.lock.grab() // mutex
    vector->m_count++;
    vector->m_handler(vector, inProc, saveArea);
  }
  
  if (inProc)
  {
    assert (OCPU::current()->m_currentProcess);
    // TODO: proc_dispatch_current();
    /* not reached */
  }
  
  // Return from kernel interrupt
  return;
}

/**
 * @brief Initialize the interrupt descriptor table
 */
void OInterruptVector::initialize()
{
  for (size_t i = 0; i < NUM_VECTOR; i++)
  {
    m_vectorMap[i].m_type     = vt_Unbound;
    m_vectorMap[i].m_handler  = OVectorHandlers::UnboundVector;
    m_vectorMap[i].m_status   = vs_Uninitialized;
  }
  
  for (size_t i = 0; i < NUM_TRAP; i++)
  {
    m_vectorMap[i].m_type     = vt_HardwareTrap;
    m_vectorMap[i].m_mode     = vm_FromBUS;
    m_vectorMap[i].m_level    = vl_FromBUS;
    m_vectorMap[i].m_status   = vs_Live;
    m_vectorMap[i].m_handler  = OVectorHandlers::ReservedException;
  }
  
  m_vectorMap[vec_DivZero].m_handler            = OVectorHandlers::UserFault;
  m_vectorMap[vec_Debug].m_handler              = OVectorHandlers::DebugException;
  m_vectorMap[vec_NMI].m_handler                = OVectorHandlers::FatalFault;
  m_vectorMap[vec_BreakPoint].m_handler         = OVectorHandlers::BptTrap;
  m_vectorMap[vec_Overflow].m_handler           = OVectorHandlers::UserFault;
  m_vectorMap[vec_Bounds].m_handler             = OVectorHandlers::UserFault;
  m_vectorMap[vec_BadOpcode].m_handler          = OVectorHandlers::UserFault;
  m_vectorMap[vec_DeviceNotAvail].m_handler     = OVectorHandlers::DeviceNotAvailable;
  m_vectorMap[vec_DoubleFault].m_handler        = OVectorHandlers::FatalFault;
  m_vectorMap[vec_CoprocessorOverrun].m_handler = OVectorHandlers::UnboundVector;
  m_vectorMap[vec_InvalidTSS].m_handler         = OVectorHandlers::FatalFault;
  m_vectorMap[vec_SegNotPresent].m_handler      = OVectorHandlers::UserFault;
  m_vectorMap[vec_StackSeg].m_handler           = OVectorHandlers::UserFault;
  m_vectorMap[vec_GeneralProtection].m_handler  = OVectorHandlers::UserFault;
  m_vectorMap[vec_PageFault].m_handler          = OVectorHandlers::PageFault;
  m_vectorMap[vec_CoprocError].m_handler        = OVectorHandlers::FatalFault;
  m_vectorMap[vec_AlignCheck].m_handler         = OVectorHandlers::UserFault;
  m_vectorMap[vec_MachineCheck].m_handler       = OVectorHandlers::FatalFault;
  m_vectorMap[vec_SIMDfp].m_handler             = OVectorHandlers::UserFault;
  
  // Some of the exception vectors have associated instructions that can be executed from user mode:
  m_vectorMap[vec_BreakPoint].m_user  = true;
  m_vectorMap[vec_Overflow].m_user    = true;
  m_vectorMap[vec_Bounds].m_user      = true;
  
  // Setup the syscall
  m_vectorMap[vec_Syscall].m_type     = vt_SystemCall;
  m_vectorMap[vec_Syscall].m_status   = vs_Live;
  m_vectorMap[vec_Syscall].m_user     = true;
  m_vectorMap[vec_Syscall].m_handler  = OVectorHandlers::SysCall;
  
  assert(sizeof(IdtTable) == sizeof(IdtTable[0])*NUM_VECTOR);
  
  // Initialize all hardware idtable entries
  for (size_t vec = 0; vec < NUM_VECTOR; vec ++)
    setHardwareVector(vec, irq_stubs[vec], m_vectorMap[vec].m_user);
  
  descriptor_table_pointer_t IDTD;
  IDTD.size = sizeof(IdtTable);
  IDTD.lowAddr = reinterpret_cast<uint32_t>(IdtTable);
  
  // Load IDT
  GNU_ASM("lidt %0\n"
  : /* no output */
  : "m" (IDTD)
  : "memory"
         );
}
