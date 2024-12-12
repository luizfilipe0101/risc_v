#include <stdio.h>
#include <stdint.h>
#include "alu.h"
#include "branch.h"
#include "defines.h"
#include "log.h"
#include "system_functions.h"


uint32_t opcode = 0;


int check_type(int32_t instr, mem *flash, reg *regs, int16_t *pc)
{

  int32_t fields[6] = {0};
  
  opcode = instr & 0x7F;

  switch(opcode)
  {
    case 103: /* JALR */
    case ecall: /* ECALL */
    case itype:
      fields[rd]     = (instr >> 07) & 0x1F;
      fields[func3]  = (instr >> 12) & 0x07;
      fields[rs1]    = (instr >> 15) & 0x1F;
      fields[func7]  = (instr >> 25) & 0x7F;
      fields[imm]    = (instr >> 31) & 1 ? ((instr >> 20) | 0xFFFFF000) : (instr >> 20);        
      
      if(opcode == itype)
        imediate(fields, regs);
      
      if(opcode == 103)
      {
        log_instr(instr, opcode, fields, regs);
        return jump(opcode, fields, regs, pc);
      }

      if(opcode == ecall)
      {
        log_instr(instr, opcode, fields, regs);
        return system_rv32i(regs);
        break;
      }

      break;
      
    case ltype: /*Loads with Itype format*/ 
      fields[rd]    = (instr >> 07) & 0x1F;
      fields[func3] = (instr >> 12) & 0x07;
      fields[rs1]   = (instr >> 15) & 0x1F;
      fields[imm]   = (instr >> 25) & 0x7F;
      
      /* Signed Extended*/
      if(((instr >> 31) & 1) == 1)
        fields[4] = (instr >> 20) | 0xFFFFF000;
        
      else
        fields[4] = (instr >> 20);

      load(fields, flash, regs);
      break;

    case stype:
      fields[rd] = ( instr >> 07  & 0x1F);
      fields[func3] = ((instr >> 12) & 0x07);
      fields[rs1] = ((instr >> 15) & 0x1F);
      fields[rs2] = ((instr >> 20) & 0x1F);

      fields[imm] |= (instr & 0x80000000) >> 20;
      fields[imm] |= (instr & 0x7E000000) >> 20;
      fields[imm] |= (instr & 0xF00)      >> 07;
      fields[imm] |= (instr & 0x80)       >> 07;

      store(flash, regs, fields);
      break;
      
    case rtype:
      fields[rd]  = (instr >> 07) & 0x1F;    // Operações que preservam os bits úteis e zeram os demais //
      fields[func3]  = (instr >> 12) & 0x07;
      fields[rs1]  = (instr >> 15) & 0x1F;
      fields[rs2]  = (instr >> 20) & 0x1F;
      fields[func7]  = (instr >> 25) & 0x7F;
      regist(fields, regs);
      break;
      
    case btype:
      fields[rd]    = (instr >> 07) & 0x1F;
      fields[func3] = (instr >> 12) & 0x07;
      fields[rs1]   = (instr >> 15) & 0x1F;
      fields[rs2]   = (instr >> 20) & 0x1F;

      fields[imm]  |= (instr & 0x80000000) >> 19;
      fields[imm]  |= (instr & 0x80)       << 4;
      fields[imm]  |= (instr & 0x7E000000) >> 20;
      fields[imm]  |= (instr & 0xF00)      >> 7;

      log_instr(instr, opcode, fields, regs);

      return branch(fields, regs);

      break;
    
    /* U-type */
    case 23:
    case 55:  /* LUI */ 
      int32_t op = 0;
      fields[rd] = (instr >> 7) & 0x1F;
      fields[imm] = ((signed int )instr >> 12);
      op = instr & 0x7F;
      upper(op, fields, regs, *pc);
      break;

    case jtype:
      fields[rd]   = (instr >> 7)  & 0x1F;
      
      fields[imm] |= (instr & 0x80000000) >> 11;  // MSB
      fields[imm] |= (instr & 0xFF000);
      fields[imm] |= (instr & 0x100000)   >> 9;
      fields[imm] |= (instr & 0x7E000000) >> 20;
      fields[imm] |= (instr & 0x1E00000)  >> 20;

      log_instr(instr, opcode, fields, regs);

      return jump(opcode, fields, regs, pc);
      
      break;
      
    default:
      fprintf(stderr, "RV32I - Internal Segmentation Fault\n");
      return -1;      
  }

  log_instr(instr, opcode, fields, regs);

  return 0;

}

