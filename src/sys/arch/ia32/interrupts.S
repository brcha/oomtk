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
/** @file interrupts.S
 * @brief Low-level ia32 interrupt handlers
 */

#include <ia32/asm.h>
#include <ia32/pagesize.h>
#include <ia32/selectors.h>
#include <ia32/irq.h>

#include <macros.h>
#include <config.h>

#include "arch/ia32/offsets.h"

	/* Define a trap entry code for a trap that does not push an error code */
#define DEFENTRY(vecno)		   \
	.text			 ; \
1:	pushl	$0		 ; \
	pushl	$vecno		 ; \
	jmp	EXT(intr_common) ; \
	.data			 ; \
	.long	1b

	/* Define a trap entry for a trap that does push an error code */
#define DEFENTRY_EC(vecno, label) \
	.text			; \
1:	pushl	$vecno		; \
	jmp	EXT(label)	; \
	.data			; \
	.long	1b

	.data
ENTRY(irq_stubs)
	/* Set up the hardware trap entry stubs: */
DEFENTRY(0x00)			/* #DE: Divide error				*/
DEFENTRY(0x01)			/* #DB: Debug exception				*/
DEFENTRY(0x02)			/* NMI						*/
DEFENTRY(0x03)			/* #BP: Breakpoint				*/
DEFENTRY(0x04)			/* #OF: Overflow (from INTO)			*/
DEFENTRY(0x05)			/* #BR: Bounds error (from BOUND)		*/
DEFENTRY(0x06)			/* #UD: Invalid opcode				*/
DEFENTRY(0x07)			/* #NM: Device (coprocessor) not available	*/
DEFENTRY(0x08)			/* #DF: Double fault				*/
DEFENTRY(0x09)			/* Coprocessor segment overrun (not used on	*
				 * recent processors, only on 387 coprocessor	*/
/* If Invalid TSS happends in the kernel return path, it will never be seen:	*/
DEFENTRY_EC(0x0a, intr_common)	/* #TS: Invalid TSS				*/
DEFENTRY_EC(0x0b, maybe_iret)	/* #NP: Segment not present			*/
DEFENTRY_EC(0x0c, maybe_iret)	/* #SS: Stack fault				*/
DEFENTRY_EC(0x0d, maybe_iret)	/* #GP: General protection fault		*/
DEFENTRY_EC(0x0e, maybe_iret)	/* #PF: Page fault				*/
DEFENTRY(0x0f)			/* Intel reserved, do not use			*/
DEFENTRY(0x10)			/* #MF: x87 floating point error		*/
DEFENTRY_EC(0x11, maybe_iret)	/* #AC: Alignment check				*/
DEFENTRY(0x12)			/* #MC: Machine check				*/
DEFENTRY(0x13)			/* #XC: SIMD floating point error		*/
/* 0x13 is the last defined hardware trap (by intel's manual from 15th november *
 * 2006). The rest of the entries (0x14-0x1f) are reserved and are not for use, *
 * so they will be defined here and handled as unknown (and hopefully, they 	*
 * will never occur).								*/
DEFENTRY(0x14)
DEFENTRY(0x15)
DEFENTRY(0x16)
DEFENTRY(0x17)
DEFENTRY(0x18)
DEFENTRY(0x19)
DEFENTRY(0x1a)
DEFENTRY(0x1b)
DEFENTRY(0x1c)
DEFENTRY(0x1d)
DEFENTRY(0x1e)
DEFENTRY(0x1f)

vecno=NUM_TRAP

	/* Setup the interrupt entry stubs (32-255). */
	.text
.rept NUM_IRQ
DEFENTRY(vecno)
vecno=vecno+1
.endr

	/* On entry to intr_common, the stack frame depends on whether it was a ring0 (kernel) or ring3 (user)
	 * trap. For ring3 (or any ring other than ring0), the stack looks like this:
	 *
	 *  ??  SS
	 *   ESP
	 *  EFLAGS
	 *  ??  CS
	 *   EIP
	 *   CODE	(because of the trap stub)
	 *   VECNO	(because of the trap stub)
	 *
	 * For ring0, the stack looks like this:
	 *
	 *  EFLAGS
	 *  ??  CS
	 *   EIP
	 *   CODE 	(because of the trap stub)
	 *   VECNO	(because of the trap stub)
	 *
	 * CS and SS have been reloaded by the hardware, but DS (and rest of the data segment registers)
	 * is yet to be reloaded.
	 */
	.text
	.align 16
LEXT(intr_common)
	/* Save all the registers */
	pusha
	/* Save %cr2 value (maybe it was a page fault, and if it wasn't it doesn't hurt) */
	movl	%cr2, %eax
	movl	%eax, OFFSETOF_Fixed_PFAddr(%esp)

	testl	$3, OFFSETOF_Fixed_CS(%esp)
	jz	L_kernel_interrupt

	/* No, this was a user-mode interrupt, so save everything before the switch to kernel stack */
	movl	%esp, %ebp		/* Pointer to save area in %ebp	*/
	mov	%gs, OFFSETOF_Fixed_GS(%ebp)
	mov	%fs, OFFSETOF_Fixed_FS(%ebp)
	mov	%ds, OFFSETOF_Fixed_DS(%ebp)
	mov	%es, OFFSETOF_Fixed_ES(%ebp)

	/* Load the kernel segment register values */
	mov	$sel_KernelData, %ax
	mov	%ax, %ds
	mov	%ax, %es

	subl	$OFFSETOF_OProcess_FixedRegs, %ebp

	/* Reload per-CPU kernel stack */
	movl	OFFSETOF_OProcess_OnCPU(%ebp), %esp
	movl	OFFSETOF_OCPU_stackTop(%esp), %esp

	pushl	%ebp	/* Pointer to save area */
	addl	$OFFSETOF_OProcess_FixedRegs, (%esp)

	pushl	%ebp	/* Pointer to process structure */

	/* Call generic user-mode interrupt/trap handler */
	call	EXT(irq_OnTrapOrInterrupt)

	/* The above call will never return, but it doesn't hurt to make sure */
1:	hlt
	jmp	1b

L_kernel_interrupt:
	/* This was a kernel interrupt. */
	pushl	%esp	/* pointer to save area			*/
	pushl	$0	/* pointer to process (= NULL)		*/
	/* Call generic kernel-mode interrupt/trap handler	*/
	call	EXT(irq_OnTrapOrInterrupt)

	addl	$8, %esp	/* unwind the arguments	*/
	/* Return immediately to interrupted context	*/
	popa			/* restore registers	*/
	addl	$8, %esp	/* skip error, vecno	*/
L_kernel_iret:
	iret			/* return to kernel	*/

	.text
ENTRY(scheduler_low_level_yield)
	andl	$KSTACK_MASK, %esp
	addl	$KSTACK_SIZE, %esp
	call	EXT(scheduler_dispatch_something)

	/* Put the processor into it's idle mode until the next interrupt appears. */
	.text
ENTRY(IdleThisProcessor)
1:	hlt
	jmp	1b

	/* This is called with interrupts disabled */
ENTRY(asm_proc_resume)
	movl	4(%esp), %esp	/* current process pointer */
	/* For now, simply turn off issue_SysCallDone. */
	movl	$0, OFFSETOF_OProcess_Issues(%esp)
	leal	OFFSETOF_OProcess_FixedRegs(%esp), %esp
	/* Pop saved registers */
	popa
	/* Skip the error & exception numbers */
	addl	$8, %esp
	/* NOTE: Each of the following loads can fail if user has established a invalid
	 * segment selector value (including Null selector for SS during IRET). That will
	 * cause #GP and later it will be checked if it was caused by one of these loads,
	 * fix the saveArea.
	 */
L_reload_es:
	mov	OFFSETOF_Fixed_ES - OFFSETOF_Fixed_EIP(%esp), %es
L_reload_ds:
	mov	OFFSETOF_Fixed_DS - OFFSETOF_Fixed_EIP(%esp), %ds
L_reload_fs:
	mov	OFFSETOF_Fixed_FS - OFFSETOF_Fixed_EIP(%esp), %fs
L_reload_gs:
	mov	OFFSETOF_Fixed_GS - OFFSETOF_Fixed_EIP(%esp), %gs

	/* Segments have been reloaded => return */
L_iret:
	iret

	/* This sub-handler checks if the fault occured during iret */
maybe_iret:
	/* Faults on the iret path happen only in kernel-mode */
	testl	$3, OFFSETOF_Trap_CS(%esp)
	jnz	intr_common
maybe_kernel_iret:
	/* Check if any of the reload instructions need recovery */
	cmpl	$L_reload_es, OFFSETOF_Trap_EIP(%esp)
	je	iret_recover
	cmpl	$L_reload_ds, OFFSETOF_Trap_EIP(%esp)
	je	iret_recover
	cmpl	$L_reload_fs, OFFSETOF_Trap_EIP(%esp)
	je	iret_recover
	cmpl	$L_reload_gs, OFFSETOF_Trap_EIP(%esp)
	je	iret_recover
	cmpl	$L_iret, OFFSETOF_Trap_EIP(%esp)
	je	iret_recover
	/* Nope, then procede with common interrupt handler */
	jmp	intr_common

	/* Recover from IRET interrupt:
	 *
	 * 5 instructions can cause exception: popl %{e,d,f,g}s and iret.
	 * The exception that can be generated are:
	 *	#GP - if code segment is wrong
	 *	#SS - if stack segment is wrong
	 * 	#NP - if stack segment is not present
	 *	#TS - if returning to invalid task segment
	 *	#AC - if alignment checking enabled
	 *	#PF - if instruction page is not present
	 *
	 * If such a exception occurs, it will push 5 word before getting to here:
	 *	exception number
	 *	error code
	 *	eip
	 *	cs
	 *	eflags
	 *
	 * The trick is to patch the stack so that it looks like exception was generated by the
	 * user instruction rather than the return path. The 5 values are written on top of old
	 * values, so they must be recovered, and here is how they are overwritten:
	 *	error code (zero if none)	->	eflags
	 *	trap/interrupt number		->	kernel code cs
	 *	eax				->	eip of IRET instruction
	 *	ecx				->	error code
	 *	edx				->	vector number
	 *
	 * The registers (in CPU) should be the same as those overwritten, so that is not a problem.
	 * New error code and vector number will be placed on their rightful positions and stack pointer
	 * will be adjusted to the proper position.
	 */
iret_recover:
	/* Recover kernel segment values */
	mov	$sel_KernelData, %ax
	mov	%ax, %ds
	mov	%ax, %es

	movl	%esp, %ebp
	subl	$OFFSETOF_Fixed_EDX, %ebp

	/* %ebp -> save area base
	 * %esp -> dumped frame
	 * move the vector number and error code to expected locations.
	 */
	movl	(%esp), %ebx
	movl	%ebx, OFFSETOF_Fixed_VecNo(%ebp)
	movl	4(%esp), %ebx
	movl	%ebx, OFFSETOF_Fixed_Error(%ebp)

	/* Save %e{c,d,a}x back to the saved area */
	movl	%ecx, OFFSETOF_Fixed_ECX(%ebp)
	movl	%edx, OFFSETOF_Fixed_EDX(%ebp)
	movl	%eax, OFFSETOF_Fixed_EAX(%ebp)

	subl	$OFFSETOF_OProcess_FixedRegs, %ebp

	/* Reload kernel stack */
	movl	OFFSETOF_OProcess_OnCPU(%ebp), %esp
	movl	OFFSETOF_OCPU_stackTop(%esp), %esp
	addl	$KSTACK_SIZE, %esp

	pushl	%ebp	/* Pointer to saved area */
	addl	$OFFSETOF_OProcess_FixedRegs, (%esp)

	pushl	%ebp	/* Pointer to process structure */

	/* Call generic kernel-mode interrupt/trap handler */
	call	EXT(irq_OnTrapOrInterrupt)
