#ifndef MLC_MOD32_H__
#define MLC_MOD32_H__

/** return (divident % divisor) with divisor<2^32 */
extern inline CONST
unsigned long
mod32(unsigned long long divident, unsigned long divisor)
{
  unsigned long ret, dummy;
  asm ("divl	%5		\n\t"
       "movl	%4, %%eax	\n\t"
       "divl	%5		\n\t"
     : "=d"(ret), "=a"(dummy)
     : "a"((unsigned long)(divident >> 32)), "d"(0),
       "irm"((unsigned long)(divident & 0xffffffff)), "rm"(divisor));
  return ret;
}

#endif
