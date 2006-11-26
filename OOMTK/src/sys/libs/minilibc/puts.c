#include <stdio.h>
#include <string.h>
#include "libc_backend.h"

int puts( const char *c ) 
{  
  __libc_backend_outs(c,strlen(c)); 
  return __libc_backend_outs("\n",1); 
}

int putstr( const char *const c )
{
  return __libc_backend_outs(c,strlen(c)); 
}

int putnstr( const char *const c, int len )
{
  return __libc_backend_outs(c,len); 
}
