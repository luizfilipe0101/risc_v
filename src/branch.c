#include <stdint.h>
#include <stdio.h>
#include "defines.h"

int branch(int32_t *fields, reg *regs)
{
  
  int result = 0;

  switch(fields[func3])
  {
    case 0:  /* BEQ */
      regs[fields[rs1]].sval == regs[fields[rs2]].sval ? (result = fields[imm]) : (result = 0);
      break;
    
    case 1:   /* BNE */
      regs[fields[rs1]].sval != regs[fields[rs2]].sval ? (result = fields[imm]) : (result = 0);
      break;
      
    case 4: /* BLT */
      regs[fields[rs1]].sval < regs[fields[rs2]].sval ? (result = fields[imm]) : (result = 0);
      break;
      
    case 5: /* BGE */
      regs[fields[rs1]].sval > regs[fields[rs2]].sval ? (result = fields[imm]) : (result = 0);
      break;
      
    case 6: /* BLTU */
      break;
      
    case 7: /* BGEU */
      break;
      
    default:
      return -1;
  }
  
  return result;
  
}
