#include <stdio.h>
#include <stdint.h>
#include "defines.h"

void imediate(uint32_t *fields, uint32_t *regs)
{

  switch(fields[func3])
  {
    case addi:         /* ADDI*/
      puts("ADDI");
      regs[fields[rd]] = regs[fields[rs1]] + fields[imm];
      break;
  }
}

void regist(uint32_t *fields, uint32_t *regs)
{
  switch(fields[func3])
  {
    case 0:
      if(!func7)   /* ADD*/
      {
        puts("ADD");
        regs[fields[rd]] = regs[fields[rs1]] + regs[fields[rs2]];
      }
      else         /* SUB*/
      {
        puts("SUB");
        regs[fields[rd]] = regs[fields[rs1]] - regs[fields[rs2]];
      }
      break;
  }
}
