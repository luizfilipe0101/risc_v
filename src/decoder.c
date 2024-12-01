#include <stdio.h>
#include <stdint.h>
#include "alu.h"
#include "branch.h"
#include "defines.h"


uint32_t opcode = 0;


int check_type(uint32_t instr, uint32_t *regs)
{
  printf("Instr: %.8X\n", instr);
  uint32_t fields[6] = {0};
  /*
    rd    0
    func3 1
    rs1   2
    rs2   3
    imm   4
    func7 5
  */
  
  opcode = instr & 0x7F;
  printf("Opcode: %d\n", opcode);
  switch(opcode)
  {
    case itype:
      fields[0]  = (instr >> 7)  & 0x1F;
      fields[1]  = (instr >> 12) & 0x07;
      fields[2]  = (instr >> 15) & 0x1F;
      fields[5]  = (instr >> 25) & 0x07F;
      fields[4]  = (instr >> 20) & 0xFFF;
      printf("rd = %d\n", fields[0]);
      printf("func3 = %d\n", fields[1]);
      printf("rs1 = %d\n", fields[2]);
      printf("imm = %d\n", fields[4]);
      printf("func7 = %d\n", fields[5]);
      imediate(fields, regs);
      break;
      
    case rtype:
      fields[0]  = (instr >> 7)  & 0x1F;
      fields[1]  = (instr >> 12) & 0x07;
      fields[2]  = (instr >> 15) & 0x1F;
      fields[3]  = (instr >> 20) & 0x1F;
      fields[5]  = (instr >> 25) & 0x7F;
      printf("rd = %d\n", fields[0]);
      printf("func3 = %d\n", fields[1]);
      printf("rs1 = %d\n", fields[2]);
      printf("rs2 = %d\n", fields[3]);
      printf("func7 = %d\n", fields[5]);
      regist(fields, regs);
      break;
      
    case btype:
      puts("Branch");
      fields[0]  = (instr >> 7)  & 0x1F;
      fields[1]  = (instr >> 12) & 0x07;
      fields[2]  = (instr >> 15) & 0x1F;
      fields[3]  = (instr >> 20) & 0x1F;
      fields[5]  = (instr >> 25) & 0x7F;
      printf("rd = %d\n", fields[0]);
      printf("func3 = %d\n", fields[1]);
      printf("rs1 = %d\n", fields[2]);
      printf("rs2 = %d\n", fields[3]);
      printf("func7 = %d\n", fields[5]);
      printf("BRANCH AQUI!! = %d\n\n", branch(fields, regs));
      return branch(fields, regs);
      break;
      
    default:
      fprintf(stderr, "RV32I - Internal Segmentation Fault\n");
      return 1;
      
  }
  
  return 0;

}

