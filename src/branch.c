#include <stdint.h>
#include <stdio.h>
#include "defines.h"

int branch(uint32_t *fields, uint32_t *regs)
{
  
  int result = 0;

  switch(fields[func3])
  {
    case beq:
      regs[fields[rs1]] == regs[fields[rs2]] ? (result = fields[rd]) : (result = 0);
      break;
    
    case bne:
      regs[fields[rs1]] != regs[fields[rs2]] ? (result = fields[rd]) : (result = 0);
      break;
      
    case blt:
      regs[fields[rs1]] < regs[fields[rs2]] ? (result = fields[rd]) : (result = 0);
      break;
      
    case bge:
      regs[fields[rs1]] > regs[fields[rs2]] ? (result = fields[rd]) : (result = 0);
      break;
      
    case bltu:
      break;
      
    case bgeu:
      break;
      
    default:
      return -1;
  }
  
  return result;
  
}
