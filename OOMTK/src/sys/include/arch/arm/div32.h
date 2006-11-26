#ifndef MLC_DIV32_H__
#define MLC_DIV32_H__

/** return (divident / divisor) with divisor<2^32 */
extern inline CONST
unsigned long long
div32(unsigned long long divident, unsigned long divisor)
{
  return divident/divisor;
}

#endif
