#include <stdio.h>
#include <stdint.h>
#include "defines.h"
#include "controller.h"
#include "architecture.h"


int32_t alu(int32_t src1, int32_t src2, uint8_t ctrl)
{

  switch(ctrl)
  {
    case 0:
    case 1:
      // SUB = src1 + TWO'S(src2)
      return ctrl != 0 ? (src1 + (~(src2)+1)) : (src1 + src2);
      break;
    
    case 2:
      return src1 & src2;
      break;

    case 3:
      return src1 | src2;
      break;

    case 5:
      return src1 < src2 ? (1) : (0);
      break;

    default:
      return 0;
      break;
    
  }
}