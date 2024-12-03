#include <stdio.h>
#include <stdint.h>
#include "defines.h"

void imediate(int32_t *fields, reg *regs)
{

  switch(fields[func3])
  {
    case addi:         /* ADDI*/
      puts("ADDI");
      regs[fields[rd]].sval = regs[fields[rs1]].sval + fields[imm];
      break;
      
                /*********************** A testar ... V  **************************/
      
    case 2:         /* SLTI*/
      puts("SLTI");
      regs[fields[rd]].sval = regs[fields[rs1]].sval < fields[imm] ? 1 : 0;
      break;
      
    case 3:         /* SLTIU*/
      puts("SLTIU");
      regs[fields[rd]].uval = (uint32_t)regs[fields[rs1]].sval < (uint32_t)fields[imm] ? 1 : 0;
      break;
      
    case 4:         /* XOR*/
      puts("XORI");
      regs[fields[rd]].sval = regs[fields[rs1]].sval ^ fields[imm];
      break;
      
    case 6:         /* ORI*/
      puts("ORI");
      regs[fields[rd]].sval = regs[fields[rs1]].sval | fields[imm];
      break;
      
    case 7:         /* ANDI*/
      puts("ANDI");
      regs[fields[rd]].sval = regs[fields[rs1]].sval & fields[imm];
      break;
      
    case 1:         /* SLLI*/
      puts("SLLI");
      regs[fields[rd]].sval = regs[fields[rs1]].sval << (fields[imm] & 0x1F);
      break;
      
    case 5:         /* SRLI*/
      if(!((fields[imm] >> 11) & 1))
      {
        puts("SRLI");
        regs[fields[rd]].sval = regs[fields[rs1]].sval >> (fields[imm] & 0x1F);
      }
      else        /* SRAI */
      {
        puts("SRAI");
        regs[fields[rd]].uval = (uint32_t)regs[fields[rs1]].sval >> (fields[imm] & 0x1F);
      }
      break;
      
    
  }
}

void regist(int32_t *fields, reg *regs)
{
  switch(fields[func3])
  {
    case 0:
      if(!func7)   /* ADD */
      {
        puts("ADD");
        regs[fields[rd]].sval = regs[fields[rs1]].sval + regs[fields[rs2]].sval;
      }
      else         /* SUB */
      {
        puts("SUB");
        regs[fields[rd]].sval = regs[fields[rs1]].sval - regs[fields[rs2]].sval;
      }
      break;
      
    case 1:        /* SLL */
      puts("SLL");
      regs[fields[rd]].sval = (regs[fields[rs1]].sval << (regs[fields[rs2]].sval &= 0x1F));
      break;
    
    case 2:       /* SLT */
      puts("SLT");
      regs[fields[rd]].sval = regs[fields[rs1]].sval < regs[fields[rs2]].sval ? 1 : 0;
      break;
    
    case 3:       /* SLTU */
      puts("SLTU");
      regs[fields[rd]].uval = (uint32_t)regs[fields[rs1]].uval < (uint32_t)regs[fields[rs2]].uval ? 1 : 0;
      break;
                /*********************** A testar ... V  **************************/
    case 4:         /* XOR */
      puts("XOR");
      regs[fields[rd]].sval = regs[fields[rs1]].sval ^ regs[fields[rs2]].sval;
      break;
      
    case 5:        /* SRL */
      if(!fields[func7])
      {
        puts("SRL");
        regs[fields[rd]].sval = regs[fields[rs1]].sval >> (regs[fields[rs2]].sval &= 0x1F);
      }
      else
      {
        puts("SRA");
        regs[fields[rd]].sval = (uint32_t)regs[fields[rs1]].sval >> (regs[fields[rs2]].sval &= 0x1F);
      }    
      break;
      
    case 6:     /* OR */
      puts("OR");
      regs[fields[rd]].sval = regs[fields[rs1]].sval | regs[fields[rs2]].sval;
      break;
        
    case 7:     /* AND */
      puts("AND");
      regs[fields[rd]].sval = regs[fields[rs1]].sval & regs[fields[rs2]].sval;
      break;
    
  }
}
