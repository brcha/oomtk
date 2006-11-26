#include "assert.h"
#include "stdio.h"
#include "stdlib.h"

void
__assert_fail (const char *__assertion, const char *__file,
	       unsigned int __line)
{
  printf("ASSERTION_FAILED (%s)\n"
	 "  in file %s:%d\n", __assertion, __file, __line);
  exit (EXIT_FAILURE);
}
