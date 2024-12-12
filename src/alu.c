#include <stdio.h>
#include <stdint.h>
#include "defines.h"

void imediate(int32_t *fields, reg *regs)
{ 

  switch(fields[func3])
  {
    
    case 0:         /* ADDI*/
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
        regs[fields[rd]].sval = (uint32_t)regs[fields[rs1]].sval >> (fields[imm] & 0x1F);
      }
      else        /* SRAI */
      {
        puts("SRAI");
        regs[fields[rd]].uval = regs[fields[rs1]].sval >> (fields[imm] & 0x1F);
      }
      break;
      
    
  }
}// immediate

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
        regs[fields[rd]].sval = (uint32_t)regs[fields[rs1]].sval >> (regs[fields[rs2]].sval &= 0x1F);
      }
      else
      {
        puts("SRA");
        regs[fields[rd]].sval = regs[fields[rs1]].sval >> (regs[fields[rs2]].sval &= 0x1F);
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
}// end regist

void load(int32_t *fields, mem *flash, reg *regs)
{
  int32_t temp = 0;

  switch(fields[func3])
  {
    case 0:     /* LB */
      regs[fields[rd]].sval = (int32_t)flash[ fields[rs1] + fields[imm] ].sval;
      break;
      
    case 1:     /* LH */
      temp |= (int32_t)flash[ fields[rs1] + fields[imm] ].sval;
      temp |= (int32_t)( flash[(regs[fields[rs1]].sval + fields[imm])+1].sval ) << 8;
      regs[fields[rd]].sval = temp;
      break;
      
    case 2:    /* LW */ 
      puts("LW");
      temp |= (int32_t)flash[regs[fields[rs1]].sval + fields[imm]].sval;
      temp |= ((int32_t)flash[regs[fields[rs1]].sval + fields[imm]+1].sval << 8);
      temp |= ((int32_t)flash[regs[fields[rs1]].sval + fields[imm]+2].sval << 16);
      temp |= ((int32_t)flash[regs[fields[rs1]].sval + fields[imm]+3].sval << 24);
      
      regs[fields[rd]].uval = temp;
      break;
      
    case 4:   /* LBU */
      regs[fields[rd]].sval = (uint8_t)flash[regs[fields[rs1] + fields[imm]].sval].uval;
      break;
      
    case 5:   /* LHU */
      temp |= flash[regs[fields[rs1]].uval + fields[imm]].uval;
      temp |= ((int32_t)flash[regs[fields[rs1]].uval + fields[imm]+1].uval << 8);
      regs[fields[rd]].uval = (uint32_t)temp;
      break;
  }

}// end load

void upper(char opcode, int32_t *fields, reg *regs, uint16_t pc)
{
  printf(" #### OPCODE: %d\n", opcode);
  if(opcode == 55)
  {
    int32_t tmp;
    register int i;
    tmp = (fields[imm] << 12);

    puts("NEW_LUI");

    for(i = 31; i >= 0; i--)
      (tmp >> i) & 1 ? putchar('1') : putchar('0');
    putchar(10);
    
    puts("LUI");
    regs[fields[rd]].sval = (fields[imm] << 12);

  }else
  {
    puts("AUIPC");
    regs[fields[rd]].sval = pc + (uint16_t)fields[imm];
  }
  

}//end upper

void store(mem *flash, reg *regs, int32_t *fields)
{
  register int i = 0;
  int32_t new_imm = 0;
  new_imm = fields[rd];
  new_imm |= (fields[imm] << 6);

  puts("NEW_IMM");
  for(i = 31; i >= 0; i--)
    (new_imm >> i) & 1 ? putchar('1') : putchar('0');
  putchar(10);

  switch(fields[func3])
  {
    case 0:
      puts("[SB]");
      flash[(regs[fields[rs1]].sval + new_imm) & 0xFFFF].sval = (regs[fields[rs2]].sval & 0xFF);
    break;

    case 1:
      puts("[SH]");
      flash[(regs[fields[rs1]].sval + new_imm) & 0xFFFF].sval = (regs[fields[rs2]].sval & 0xFF);
      flash[(regs[fields[rs1]].sval + (new_imm + 1)) & 0xFFFF].sval = (regs[fields[rs2]].sval >> 8) & 0xFF;
    break;

    case 2:
      puts("[SW]");
      flash[(regs[fields[rs1]].sval + new_imm) & 0xFFFF].sval = (regs[fields[rs2]].sval & 0xFF);
      flash[(regs[fields[rs1]].sval + (new_imm + 1)) & 0xFFFF].sval = (regs[fields[rs2]].sval >> 8) & 0xFF;
      flash[(regs[fields[rs1]].sval + (new_imm + 2)) & 0xFFFF].sval = (regs[fields[rs2]].sval >> 16) & 0xFF;
      flash[(regs[fields[rs1]].sval + (new_imm + 3)) & 0xFFFF].sval = (regs[fields[rs2]].sval >> 24) & 0xFF;
    break;
  }
}// end store

int16_t jump(int32_t *fields, reg *regs, int16_t pc)
{
  puts("JAL!!!");
  regs[fields[rd]].sval = pc + 4;
  return fields[imm];
}
