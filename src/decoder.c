#include <stdio.h>
#include <stdint.h>
#include "alu.h"
#include "branch.h"
#include "defines.h"
#include "log.h"


uint32_t opcode = 0;


int check_type(int32_t instr, mem *flash, reg *regs, int16_t *pc)
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
  printf("OPCODE NOW: %d\n", opcode);

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
      fields[rd]  = (instr >> 7)  & 0x1F;
      fields[func3]  = (instr >> 12) & 0x07;
      fields[rs1]  = (instr >> 15) & 0x1F;
      fields[rs2]  = (instr >> 20) & 0x1F;

      fields[imm]  |= (instr & 0x80000000) >> 19;
      fields[imm]  |= (instr & 0x80) << 4;
      fields[imm]  |= (instr & 0x7E000000) >> 20;
      fields[imm]  |= (instr & 0xF00) >> 7;

      log_instr(instr, opcode, fields, regs);

      return branch(fields, regs);

      break;
    
    /* U-type */
    case 23:
    case 55:  /* LUI */ 
      puts("UType");
      int32_t op = 0;
      fields[0] = (instr >> 7) & 0x1F;
      fields[4] = ((signed int )instr >> 12);
      op = instr & 0x7F;
      upper(op, fields, regs, *pc);
      break;

    case jtype:
      register int i;

      fields[rd]  = (instr >> 7)  & 0x1F;
      
      fields[imm] |= (instr & 0x80000000) >> 11;  // MSB
      fields[imm] |= (instr & 0xFF000);
      fields[imm] |= (instr & 0x100000) >> 9;
      fields[imm] |= (instr & 0x7E000000) >> 20;
      fields[imm] |= (instr & 0x1E00000) >> 20;

      
      puts("JAL BIN:");
      for(i = 31; i >= 0; i--)
        (instr >> i) & 1 ? putchar('1') : putchar('0');

      putchar(10); 

      puts("JAL IMM");
      for(i = 31; i >= 0; i--)
        (fields[imm] >> i) & 1 ? putchar('1') : putchar('0');

      putchar(10); 

      int16_t result =  jump(fields, regs, *pc);

      printf("JUMP RETURN VALUE: %d\n",result);

      log_instr(instr, opcode, fields, regs);

      return result;
      
      break;
      
    default:
      fprintf(stderr, "RV32I - Internal Segmentation Fault\n");
      return -1;      
  }

  log_instr(instr, opcode, fields, regs);

  return 0;

}

