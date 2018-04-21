#include "Logic_Math.h"

//--------------------------------------------------
signed char getBit(signed long num, signed char bit)
{
  return !!(num &= (1 << bit));
}
//--------------------------------------------------
signed long setBit(signed long num, signed char bit)
{
  return (num |= (1 << bit));
}
//--------------------------------------------------
signed long clrBit(signed long num, signed char bit)
{
  return (num &= ~(1 << bit));
}
//--------------------------------------------------
