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
/** @file start.S
 * @brief Bootstrap code for ia32
 */

#include <ia32/asm.h>
#include <ia32/pagesize.h>
#include <config.h>

#include <ia32/EFLAGS.h>
#include <ia32/CPUID.h>
#include <ia32/CR.h>
#include <ia32/PTE.h>

#include <multiboot.h>

	.text
GEXT(start)
GEXT(_start)
	jmp	over
	/* Multiboot header must be in the first 8192 bytes of the
	 * OS image, so it goes right after start.
	 */
	.align	32
multiboot_header:	/* magic, flags, checksum */
	.long	MULTIBOOT_HEADER_MAGIC
	.long	MULTIBOOT_HEADER_FLAGS
	.long	-(MULTIBOOT_HEADER_MAGIC+MULTIBOOT_HEADER_FLAGS)

over:
#if	HAVE_CONSOLE
	movl	$0x07740753, 0x000b8000	/* "St" */
#endif
	/* Save multiboot signature */
	movl	%eax, %edx
	cld

	/* Clear the BSS */
	movl	$EXT(_bss_start), %edi	/* start */
	movl	$EXT(_end), %ecx	/* end	 */
	subl	%edi, %ecx		/* count */
	subl	$KVA, %edi		/* no paging yet */
	xorl	%eax, %eax		/* value */
	rep
	stosb

	/* Clear the mappings from ldscript.S */
	movl	$EXT(__begin_maps), %edi
	movl	$EXT(__end_maps), %ecx
	subl	%edi, %ecx
	subl	$KVA, %edi
	xorl	%eax, %eax
	rep
	stosb

	/* Save the multiboot info (see init.cpp for more info) */
	movl	$EXT(multibootSignature)-KVA, %esi
	movl	%edx,(%esi)
	movl	$EXT(multibootInfo)-KVA, %esi
	movl	%ebx,(%esi)

	/* Check for CPUID support and PAE */
	movl	$EXT(cpu0_kstack_hi)-KVA, %esp	/* tmp stack */

	/* Check the AC bit */
	pushf
	xorl	$EFLAGS_AC,(%esp)
	movl	(%esp), %eax
	popf
	pushf
	cmpl	(%esp),%eax
	je	1f

	/* This is 486 or even worse... :( */
	pushl	$EXT(message_prehistoric)-KVA
	call	fail_message

1:	/* Restore eflags */
	pushl	%eax
	popf

	/* Check (cpu)ID bit */
	pushf
	xorl	$EFLAGS_ID, (%esp)
	movl	(%esp), %eax
	popf
	pushf
	cmpl	(%esp), %eax
	je	1f

	pushl	$EXT(message_no_cpuid)-KVA
	call	fail_message

1:	/* Restore eflags */
	pushl	%eax
	popf

	/* Check cpuid feature info */
	xorl	%eax, %eax
	cpuid
	cmp	$0, %eax
	jg	1f

	pushl	$EXT(message_no_cpuid_feature_info)-KVA
	call	fail_message

1:	/* Determine cpuid features */
	movl	$1, %eax
	cpuid

	/* Enable some features in %cr4 */
	movl	%cr4, %ebx

	/* PGE */
	movl	%edx, %eax
	andl	$CPUID_EDX_PGE, %eax
	jz	1f
	orl	$CR4_PGE, %ebx

1:	/* PSE */
	movl	%edx, %eax
	andl	$CPUID_EDX_PSE, %eax
	jz	1f
	movb    $1, EXT(HavePSE)
	orl	$CR4_PSE, %ebx

1:	/* DE */
	movl	%edx, %eax
	andl	$CPUID_EDX_DE, %eax
	jz	1f
	orl	$CR4_DE, %ebx

1:	movl	%ebx, %cr4

	/* PAE */
	movl	%edx, %eax
	andl	$CPUID_EDX_PAE, %eax
	jnz	setup_pae_map

	/* There is no PAE, kernel is loaded at 0x01000000, and
	 * linked at 0xC1000000.
	 *
	 * Goal: identity map [0G,2M) to [0G,2M) and [3G,3G+2M).
	 * Since this legacy mode has 1024 * 4kb pages, mapping
	 * will include the first 4M.
	 */
setup_legacy_map:
	/* Identity map first 4M */
	movl	$EXT(KernPageTable)-KVA, %esi
	xorl	%edx, %edx
	orl	$(PTE_V|PTE_W|PTE_A|PTE_D), %edx

	movl	$1024, %ecx
1:	movl	%edx, (%esi)
	addl	$0x1000, %edx
	addl	$4, %esi
	loop	1b

	/* Adjust video adapter (0x000b8000) to be write-through */
	movl	$8, %ecx
	movl	$EXT(KernPageTable)-KVA+(0xb8*4), %esi
1:	orl	$(PTE_PWT|PTE_PCD), (%esi)
	addl	$4, %esi
	loop	1b

	movl	$EXT(KernPageDir)-KVA, %esi
	movl	$EXT(KernPageTable)-KVA, %edx
	orl	$(PTE_V|PTE_W|PTE_A|PTE_D), %edx
	movl	%edx, (%esi)		/* Map at VA=0  */
	movl	%edx, 3072(%esi)	/* and at VA=3G */

	/* Load KernPageDir into %cr3 */
	movl	$EXT(KernPageDir)-KVA, %edx
	mov	%edx, %cr3

#if 	HAVE_CONSOLE
	movl	$0x07720761, 0x000b8004	/* "ar" */
#endif

	jmp	enable_paging

setup_pae_map:
	/* PAE maps 2M */
	movl	$EXT(KernPageTable)-KVA, %esi
	xorl	%edx, %edx
	orl	$(PAE_V|PAE_W|PAE_A|PAE_D), %edx

	movl	$512, %ecx
1:	movl	%edx, (%esi)
	movl	$0, 4(%esi)
	addl	$8, %esi
	addl	$0x1000, %edx
	loop	1b

	/* Video adapter is write-through */
	movl	$8, %ecx
	movl	$EXT(KernPageTable)-KVA+(0xb8*8), %esi
1:	orl	$(PAE_PWT|PAE_PCD), (%esi)
	addl	$8, %esi
	loop	1b

	movl	$EXT(KernPageDir)-KVA, %esi
	movl	$EXT(KernPageTable)-KVA, %edx
	orl	$(PAE_V|PAE_W|PAE_A|PAE_D), %edx
	movl	%edx, (%esi)		/* Map at VA=0	*/
	movl	$0, 4(%esi)

	/* Now for the PDBR */
	movl	$EXT(KernPageDir)-KVA, %edx
	orl	$(PAE_V), %edx

	movl	$EXT(KernPDPT)-KVA, %esi
	movl	%edx, (%esi)		/* Map at VA=0  */
	movl	$0, 4(%esi)
	movl	$0, 8(%esi)
	movl	$0, 12(%esi)
	movl	$0, 16(%esi)
	movl	$0, 20(%esi)
	movl	%edx, 24(%esi)		/* and at VA=3G */
	movl	$0, 28(%esi)

	movl	$EXT(KernPDPT)-KVA, %edx
	mov	%edx, %cr3

#if HAVE_CONSOLE
	movl	$0x07720761, 0x000b8004	/* "ar" */
#endif

	/* Enable PAE in CR4 */
	mov	%cr4, %edx
	orl	$CR4_PAE, %edx
	mov	%edx, %cr4

enable_paging:	
#if 	HAVE_CONSOLE
	movl	$0x07690774, 0x000b8008 /* "ti" */
#endif

	/* Setup %cr0 */
	mov	%cr0, %edx
	orl	$(CR0_PG|CR0_AM|CR0_WP|CR0_ET|CR0_TS|CR0_MP|CR0_PE), %edx
#if	HAVE_CONSOLE
	/* Preserve write-through mode */
	orl	$(CR0_NW), %edx
#endif
	mov	%edx, %cr0

#if 	HAVE_CONSOLE
	movl	$0x0767076e, 0xc00b800c /* "ng" */
#endif

	/* Paging is enabled, so reload %eip */
	movl	$next_line, %edx
	jmp	*%edx
next_line:
	/* Unmap the first 2M (PAE) or 4M (legacy) */
	mov	%cr4, %edx
	andl	$CR4_PAE, %edx
	jz	drop_first_legacy_map

	/* PAE is in question, inform the kernel */
	movb	$1, EXT(UsingPAE)

	/* Remove the first PDPT entry (first 2M) */
	movl	$EXT(KernPDPT), %esi
	movl	$0, (%esi)
	movl	$0, 4(%esi)	/* this isn't needed, but looks nicer */
	jmp	reload_map

drop_first_legacy_map:
	movl	$EXT(KernPageDir), %esi
	movl	$0, (%esi)

reload_map:
	/* Reload %cr3 */
	mov	%cr3, %edx
	mov	%edx, %cr3

	/* Setup the stack for CPU0 */
	movl	$EXT(cpu0_kstack_hi), %esp

	/* Set the curCPU pointer for CPU0 */
	movl	$EXT(cpu_vector), EXT(cpu0_kstack_lo)

#if 	HAVE_CONSOLE
	movl	$0x074f0720, 0xc00b8010 /* " O" */
#endif

	/* Clear %eflags */
	pushl	$0
	popf

#if	HAVE_CONSOLE
	movl	$0x074d074f, 0xc00b8014 /* "OM" */
	movl	$0x074b0754, 0xc00b8018 /* "TK" */
#endif
	
	/* Start the kernel */
	call	EXT(arch_init)
	/* NOTE: Later this should call some kernel main... */

	/* No return, but just in case */
1:	cli
	hlt
	jmp	1b

	/* Write the fail message on console */
fail_message:
#if	HAVE_CONSOLE
	popl	%edx
	/* Clear screen */
	movb	$0x07, %ah
	movb	$0x20, %al
	movl	$0x000b8000, %edi
	movl	$(25*80), %ecx
	rep
	stosw

	movl	$0x000b8000, %edi
	movl	%edx, %esi

1:	movb	(%esi), %al
	cmpb	$0, %al
	je	halt
	movw	%ax, (%edi)
	inc	%esi
	addl	$2, %edi
	jmp	1b
#endif

ENTRY(halt)
ENTRY(sysctl_halt)
1:	cli
	hlt
	jmp	1b

#if	HAVE_CONSOLE
	.data
message_prehistoric:
	.asciz	"OOMTK doesn't support 486 or weaker processors"
message_no_cpuid:
	.asciz	"OOMTK needs CPUID support"
message_no_cpuid_feature_info:
	.asciz	"OOMTK needs CPUID that supports feature info function"
#endif
