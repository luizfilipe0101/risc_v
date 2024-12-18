/*

  ALU codes:
  000 = add       100 = slt
  001 = sub       101 = sl
  010 = and       110 = sr
  011 =  or       111 = sra

*/

#include <stdio.h>
#include <stdint.h>
#include "defines.h"
#include "controller.h"
#include "architecture.h"


int32_t alu(int32_t src1, int32_t src2, uint8_t ctrl)
{

  int32_t result = 0;

  switch(ctrl)
  {
    // Arithmetic circuit
    case 0:
      result = (src1 + src2);
      break;

    case 1:  // Sub adding TWO's(src2)
      result = (src1 + (~(src2)+1));
      break;
      
    case 5:
      result = src1 < src2 ? (1) : (0);
      break;
    
    //Logic circuit
    case 2:
      result = src1 & src2;
      break;

    case 3:
      result = src1 | src2;
      break;

    default:
      break;
  }

  // Nflag
  result < 0 ? (ctrl_reg |= 0x8) : (ctrl_reg &= ~0x8);
  // Zflag
  result == 0 ? (ctrl_reg |= 0x4) : (ctrl_reg &= ~0x4);
  // Cflag
  (result >> 31 & 1) != (rs1 >> 31 & 1) ? (ctrl_reg |= 0x2) : (ctrl_reg &= ~0x2);
  // Vflag
  result > INT32_MAX ? (ctrl_reg |= 0x1) : (ctrl_reg &= ~0x1);

  return result;
}