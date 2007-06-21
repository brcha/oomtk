/*
 *  Copyright (C) 2007 by Filip Brčić <brcha@users.sourceforge.net>
 *
 *  This file is part of OOMTK
 */
/** \file Process.h
 * \brief Process structure (platform independent part)
 */
/*
 * $Id: $
 */
#ifndef OOMTK_KernelProcess_h
#define OOMTK_KernelProcess_h

#include <Context.h>

/** \brief Process structure (platform independent)
 */

class Process
{
  public:
    Process();

    ~Process();

  protected:
    // here should be data about process, such as pointers to scheduler queues, process state, ... what ever.
    // the only thing that is here is the platform dependent context switching part:
    Context ctx;

    OFFSET_FRIENDLY;
};



#endif
