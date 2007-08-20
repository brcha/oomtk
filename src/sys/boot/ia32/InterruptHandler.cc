/*
 *  Copyright (C) 2007 by Filip Brčić <brcha@users.sourceforge.net>
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
/** \file InterruptHandler.cc
 * \brief C Interrupt handling routine
 */
#include "InterruptHandler.h"
#include "InterruptManager.h"

#include <Context.h>

#include <stdio.h>

void interruptHandler(addr_t saveArea)
{
  static InterruptManager * intManager = InterruptManager::instance();

  Context * ctx = reinterpret_cast<Context*>(saveArea);

  // intManager->handle() or something like that

  printf("Exception occured: %d\n", ctx->ExceptNo);

  printf("EDI=%08x ESI=%08x EBP=%08x Exception address=%08x\n",
	 ctx->EDI, ctx->ESI, ctx->EBP, ctx->ExceptAddr);
  printf("EBX=%08x EDX=%08x ECX=%08x EAX=%08x, ExceptNo=%08x\n",
	 ctx->EBX, ctx->EDX, ctx->ECX, ctx->EAX, ctx->ExceptNo);
  printf("Error=%08x, EIP=%08x CS=%08x EFLAGS=%08x ESP=%08x\n",
	 ctx->Error, ctx->EIP, ctx->CS, ctx->EFLAGS, ctx->ESP);
  printf("SS=%08x, DS=%08x ES=%08x FS=%08x GS=%08x\n",
	 ctx->SS, ctx->DS, ctx->ES, ctx->FS, ctx->GS);

  for (;;);
}
