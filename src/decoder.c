#include <stdio.h>
#include <stdint.h>
#include "alu.h"
#include "branch.h"
#include "defines.h"
#include "log.h"


uint32_t opcode = 0;


int check_type(uint32_t instr, mem *flash, reg *regs, uint16_t pc)
{

  int32_t fields[6] = {0};
  
  /*
    rd    0
    func3 1
    rs1   2
    rs2   3
    imm   4
    func7 5
  */
  
  opcode = instr & 0x7F;

  switch(opcode)
  {
    case itype:
      fields[0]  = (instr >> 7)  & 0x1F;
      fields[1]  = (instr >> 12) & 0x07;
      fields[2]  = (instr >> 15) & 0x1F;
      fields[5]  = (instr >> 25) & 0x7F;
      fields[4]  = (instr >> 31) & 1 ? ((instr >> 20) | 0xFFFFF000) : (instr >> 20);        
      
      imediate(fields, regs);
      break;
      
    case ltype: /*Loads with Itype format*/ 
      fields[0]  = (instr >> 7)  & 0x1F;
      fields[1]  = (instr >> 12) & 0x07;
      fields[2]  = (instr >> 15) & 0x1F;
      fields[5]  = (instr >> 25) & 0x7F;
      
      /* Signed Extended*/
      if(((instr >> 31) & 1) == 1)
        fields[4] = (instr >> 20) | 0xFFFFF000;
        
      else
        fields[4] = (instr >> 20);

      load(fields, flash, regs);
      break;

    case stype:
      fields[0] = (instr >> 7 & 0x1F);
      fields[1] = ((instr >> 12) & 0x07);
      fields[2] = ((instr >> 15) & 0x1F);
      fields[3] = ((instr >> 20) & 0x1F);
      fields[4] = ((instr >> 25) & 0x7F);
      printf("fields[0] = %d\n", fields[0]);
      printf("fields[1] = %d\n", fields[1]);
      printf("fields[2] = %d\n", fields[2]);
      printf("fields[3] = %d\n", fields[3]);
      printf("fields[4] = %d\n", fields[4]);

      store(flash, regs, fields);
      break;
      
    case rtype:
      fields[0]  = (instr >> 7)  & 0x1F;    // Operações que preservam os bits úteis e zeram os demais //
      fields[1]  = (instr >> 12) & 0x07;
      fields[2]  = (instr >> 15) & 0x1F;
      fields[3]  = (instr >> 20) & 0x1F;
      fields[5]  = (instr >> 25) & 0x7F;
      regist(fields, regs);
      break;
      
    case btype:
      puts("Branch");
      fields[0]  = (instr >> 7)  & 0x1F;
      fields[1]  = (instr >> 12) & 0x07;
      fields[2]  = (instr >> 15) & 0x1F;
      fields[3]  = (instr >> 20) & 0x1F;
      fields[5]  = (instr >> 25) & 0x7F;
      return branch(fields, regs);
      break;
    
    /* U-type */
    case 23:
      break;
    case 55:  /* LUI */ 
      char op = 0;
      puts("UType");
      fields[0] = (instr >> 7) & 0x1F;
      fields[4] = (instr >> 12);
      op |= instr & 0x1F;
      upper(op, fields, regs, pc);
      break;
      
    default:
      fprintf(stderr, "RV32I - Internal Segmentation Fault\n");
      return 1;      
  }

  log_instr(instr, opcode, fields, regs);

  return 0;

}

