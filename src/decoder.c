#include <stdio.h>
#include <stdint.h>
#include "alu.h"
#include "branch.h"
#include "defines.h"
#include "log.h"
#include "architecture.h"
#include "controller.h"
#include "system_functions.h"


uint32_t opcode   =  0;
int32_t fields[6] = {0};

int check_type(int32_t instr, reg *regs, int16_t *pc)
{
  opcode = instr & 0x7F;
  fields[rd]    = (instr >> 07) & 0x1F;
  fields[func3] = (instr >> 12) & 0x7;
  fields[rs1]   = (instr >> 15) & 0x1F;
  fields[rs2]   = (instr >> 20) & 0x1F;
  fields[func7] = (instr >> 25) & 0x7F;

  int32_t alu_res = 0;
  int32_t src1    = 0;
  int32_t src2    = 0;

  int8_t log_type = 0;

  ctrl_unit(opcode, fields[func3], fields[func7]);

  switch(opcode)
  {
    case 103: // JALR
    case itype:
    case ltype:
    case utype:
    case stype:
    case jtype:

      switch((ctrl_reg >> 5) & 0x3)                                   // ImmSrc
      {
        case 0: // I-type [31:20]
          fields[imm] = (instr >> 20);
        break;

        case 1: // S-type [31:25][11:7]
          log_type = 1;

          fields[imm]  = (instr & 0x80000000) >> 20;
          fields[imm] |= (instr & 0x7E000000) >> 20;
          fields[imm] |= (instr & 0xF00)      >> 07;
          fields[imm] |= (instr & 0x80)       >> 07;
        break;

        case 2: // U-type [31:12]
          fields[imm] = instr & 0xFFFFF000;
        break;

        case 3: // J-type [31][19:12][20][30:25][24:21][0]
          fields[imm] |= (instr & 0x80000000) >> 12;
          fields[imm] |= (instr & 0xFF000);
          fields[imm] |= (instr & 0x100000)   >>  9;
          fields[imm] |= (instr & 0x7E000000) >> 20;
          fields[imm] |= (instr & 0x1E00000)  >> 20;
        break;
      }
      
      break;
      
    case rtype:
      //regist(fields, regs);
      break;
      
    case btype:
      fields[imm]  |= (instr & 0x80000000) >> 19;
      fields[imm]  |= (instr & 0x80)       <<  4;
      fields[imm]  |= (instr & 0x7E000000) >> 20;
      fields[imm]  |= (instr & 0xF00)      >>  7;

      //log_instr(log_type, MEMORY, 0, instr, opcode, fields, regs);

      return branch(fields, regs);

      break;
          
    default:
      fprintf(stderr, "RV32I - Internal Segmentation Fault\n");
      return -99;      
  }
  
  src1 = regs[fields[rs1]].uval;
  src2 = (ctrl_reg >> 8) & 1 ? fields[imm] : regs[fields[rs2]].uval;  // ALU src (rs2 or imm)
  
  alu_res = alu(src1, src2, (uint8_t)((ctrl_reg >> 9) & 0x07));       // ALU opcode* [000]

  (ctrl_reg >> 12) & 1 ? (MEMORY[(uint16_t)alu_res].sval = regs[fields[rs2]].sval) : (0);  // WE Mem

  switch((ctrl_reg >> 13) & 0x3)                                      // Final result from ALU, Mem or Imm (result src)
  {
    case 0:
      databus = alu_res;
      printf("ALU res: %.4X\n", alu_res);
    break;

    case 1:
      switch(fields[func3])
      {
        case 0:
          databus  = ((int32_t)MEMORY[(uint16_t)alu_res].sval & 0xFF);
        break;

        case 1:
          databus  = ((int32_t)MEMORY[(uint16_t)alu_res].sval << 8);
          databus |= ((int32_t)MEMORY[((uint16_t)alu_res) + 1].sval & 0xFF);
        break;

        case 2:
          databus  = ((int32_t)MEMORY[(uint16_t)alu_res].sval << 24);
          databus |= ((int32_t)MEMORY[((uint16_t)alu_res) + 1].sval << 16);
          databus |= ((int32_t)MEMORY[((uint16_t)alu_res) + 2].sval <<  8);
          databus |= ((int32_t)MEMORY[((uint16_t)alu_res) + 3].sval & 0xFF);
        break;
      }
      
    break;

    case 2:
      databus = fields[imm];
    break;

    default:
    break;
  }

  printf("Databus: %d\n", databus);
  (ctrl_reg >> 4) & 1 ? (regs[fields[rd]].sval = databus) : (0);      // WE register file 

  log_instr(log_type, MEMORY, (uint16_t)databus, instr, opcode, fields, regs);

  return 0;

}