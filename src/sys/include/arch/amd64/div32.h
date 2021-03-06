#ifndef MLC_DIV32_H__
#define MLC_DIV32_H__

/** return (divident / divisor) with divisor<2^32 */
extern inline CONST
unsigned long long
div32(unsigned long long divident, unsigned long divisor)
{
  unsigned long long ret;
  asm ("div	%3		\n\t"
     : "=a"(ret)
     : "a"((unsigned long)(divident)), "d"(0), "rm"(divisor));
  return ret;
}

#endif
