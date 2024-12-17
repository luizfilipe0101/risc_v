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










/*

void imediate(int32_t *fields, reg *regs)
{ 

  switch(fields[func3])
  {
    
    case 0:         // ADD
      //regs[fields[rd]].sval = regs[fields[rs1]].sval + fields[imm];
      // set zero ALU CTRL   [ 000 ]
      ctrl_reg &= 0xE3FF;
      break;
      
    case 2:         // SLT
      regs[fields[rd]].sval = regs[fields[rs1]].sval < fields[imm] ? 1 : 0;
      break;
      
    case 3:         // SLTI
      regs[fields[rd]].uval = (uint32_t)regs[fields[rs1]].sval < (uint32_t)fields[imm] ? 1 : 0;
      break;
      
    case 4:         // XO
      regs[fields[rd]].sval = regs[fields[rs1]].sval ^ fields[imm];
      break;
      
    case 6:         // OR
      regs[fields[rd]].sval = regs[fields[rs1]].sval | fields[imm];
      break;
      
    case 7:         // AND
      regs[fields[rd]].sval = regs[fields[rs1]].sval & fields[imm];
      break;
      
    case 1:         // SLL
      regs[fields[rd]].sval = regs[fields[rs1]].sval << (fields[imm] & 0x1F);
      break;
      
    case 5:         // SRL
      if(!((fields[imm] >> 11) & 1))
        regs[fields[rd]].sval = (uint32_t)regs[fields[rs1]].sval >> (fields[imm] & 0x1F);
      
      else        // SRAI
        regs[fields[rd]].uval = regs[fields[rs1]].sval >> (fields[imm] & 0x1F);
      
      break;
      
    
  }
}// immediate

void regist(int32_t *fields, reg *regs)
{
  switch(fields[func3])
  {
    case 0:
      if(!func7)   // ADD
        regs[fields[rd]].sval = regs[fields[rs1]].sval + regs[fields[rs2]].sval;
      
      else         // SUB
        regs[fields[rd]].sval = regs[fields[rs1]].sval - regs[fields[rs2]].sval;
      
      break;
      
    case 1:        // SLL
      regs[fields[rd]].sval = (regs[fields[rs1]].sval << (regs[fields[rs2]].sval &= 0x1F));
      break;
    
    case 2:       // SLT
      regs[fields[rd]].sval = regs[fields[rs1]].sval < regs[fields[rs2]].sval ? 1 : 0;
      break;
    
    case 3:       // SLTU
      regs[fields[rd]].uval = (uint32_t)regs[fields[rs1]].uval < (uint32_t)regs[fields[rs2]].uval ? 1 : 0;
      break;

    case 4:         // XOR
      regs[fields[rd]].sval = regs[fields[rs1]].sval ^ regs[fields[rs2]].sval;
      break;
      
    case 5:        // SRL
      if(!fields[func7])
        regs[fields[rd]].sval = (uint32_t)regs[fields[rs1]].sval >> (regs[fields[rs2]].sval &= 0x1F);
      
      else
        regs[fields[rd]].sval = regs[fields[rs1]].sval >> (regs[fields[rs2]].sval &= 0x1F);
      break;
      
    case 6:     // OR
      regs[fields[rd]].sval = regs[fields[rs1]].sval | regs[fields[rs2]].sval;
      break;
        
    case 7:     // AND
      regs[fields[rd]].sval = regs[fields[rs1]].sval & regs[fields[rs2]].sval;
      break;
    
  }
}// end regist

void load(int32_t *fields, reg *regs)
{
  int32_t temp = 0;

  switch(fields[func3])
  {
    case 0:     // LB
      regs[fields[rd]].sval = (int8_t)MEMORY[ (uint16_t)regs[fields[rs1]].sval + fields[imm] ].sval;
      break;
      
    case 1:     // LH
      temp |= (int32_t)MEMORY[ (uint16_t)regs[fields[rs1]].sval + fields[imm] ].sval;
      temp |= (int32_t)( MEMORY[( (uint16_t)regs[fields[rs1]].sval + fields[imm])+1].sval ) << 8;
      regs[fields[rd]].sval = temp;
      break;
      
    case 2:    // LW 
      temp |= (int32_t)MEMORY[ (uint16_t)regs[fields[rs1]].sval + fields[imm]].sval;
      temp |= ((int32_t)MEMORY[ (uint16_t)regs[fields[rs1]].sval + fields[imm]+1].sval << 8);
      temp |= ((int32_t)MEMORY[ (uint16_t)regs[fields[rs1]].sval + fields[imm]+2].sval << 16);
      temp |= ((int32_t)MEMORY[ (uint16_t)regs[fields[rs1]].sval + fields[imm]+3].sval << 24);
      
      regs[(uint16_t)fields[rd]].uval = temp;
      break;
      
    case 4:   //LBU
      regs[fields[rd]].uval = (uint8_t)MEMORY[(uint16_t)regs[fields[rs1] + fields[imm]].sval].uval;
      break;
      
    case 5:   //LHU 
      temp |= MEMORY[regs[fields[rs1]].uval + fields[imm]].uval;
      temp |= (MEMORY[regs[fields[rs1]].uval + fields[imm]+1].uval << 8);
      regs[fields[rd]].uval = (uint32_t)temp;
      break;
  }

}// end load

void upper(char opcode, int32_t *fields, reg *regs, uint16_t pc)
{
  if(opcode == 55)
  {
    regs[fields[rd]].uval = (fields[imm] << 12);

  }else
    regs[fields[rd]].uval = pc + (uint16_t)fields[imm];
  
  

}//end upper

void store(reg *regs, int32_t *fields)
{

  
  switch(fields[func3])
  {
    case 0:
      MEMORY[(regs[fields[rs1]].sval + fields[imm]) & 0xFFFF].sval = (regs[fields[rs2]].sval & 0xFF);
    break;

    case 1:
      MEMORY[(regs[fields[rs1]].sval + fields[imm]) & 0xFFFF].sval = (regs[fields[rs2]].sval & 0xFF);
      MEMORY[(regs[fields[rs1]].sval + (fields[imm] + 1)) & 0xFFFF].sval = (regs[fields[rs2]].sval >> 8) & 0xFF;
    break;

    case 2:
      MEMORY[(regs[fields[rs1]].sval + fields[imm]) & 0xFFFF].sval = (regs[fields[rs2]].sval & 0xFF);
      MEMORY[(regs[fields[rs1]].sval + (fields[imm] + 1)) & 0xFFFF].sval = (regs[fields[rs2]].sval >> 8) & 0xFF;
      MEMORY[(regs[fields[rs1]].sval + (fields[imm] + 2)) & 0xFFFF].sval = (regs[fields[rs2]].sval >> 16) & 0xFF;
      MEMORY[(regs[fields[rs1]].sval + (fields[imm] + 3)) & 0xFFFF].sval = (regs[fields[rs2]].sval >> 24) & 0xFF;
    break;
  }
}// end store

int16_t jump(uint32_t opcode, int32_t *fields, reg *regs, int16_t *pc)
{
  if(opcode == jal)
  {
    regs[fields[rd]].sval = *pc + 4;
    return 0;//fields[imm];
  }else
  {
    regs[fields[rd]].uval = *pc + 4;
    return regs[fields[rs1]].uval + fields[imm];
  }
  
}// end jump

*/