#include <stdio.h>
#include <stdint.h>
#include "alu.h"
#include "branch.h"
#include "defines.h"
#include "log.h"
#include "architecture.h"
#include "controller.h"
#include "system_functions.h"


uint32_t opcode = 0;
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
  int32_t src1 = 0;
  int32_t src2 = 0;

  ctrl_unit(opcode, fields[func3], fields[func7]);

  switch(opcode)
  {
    case 103: // JALR
    case itype:
    case ltype:
    case utype:
    case stype:
    case jtype:

      switch((ctrl_reg >> 7) & 0x3)                                   // ImmSrc
      {
        case 0: // I-type [31:20]
          fields[imm] = (instr >> 20);
        break;

        case 1: // S-type [31:25][11:7]
          fields[imm] |= (instr & 0x80000000) >> 20;
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
          fields[imm] |= (instr & 0x100000) >> 9;
          fields[imm] |= (instr & 0x7E000000) >> 20;
          fields[imm] |= (instr & 0x1E00000) >> 20;
        break;
      }
      
      break;
      
    case rtype:
      //regist(fields, regs);
      break;
      
    case btype:
      fields[imm]  |= (instr & 0x80000000) >> 19;
      fields[imm]  |= (instr & 0x80)       << 4;
      fields[imm]  |= (instr & 0x7E000000) >> 20;
      fields[imm]  |= (instr & 0xF00)      >> 7;

      log_instr(instr, opcode, fields, regs);

      return branch(fields, regs);

      break;
          
    default:
      fprintf(stderr, "RV32I - Internal Segmentation Fault\n");
      return -99;      
  }

  /*
            PC target       0   
            Result source   1
            Memory write    2

            ALU control     3
            ALU control     3
            ALU control     3

            ALU Source      4

            ImmSource       5
            ImmSource       5

            RegWrite        6

            ---             000000

            000 [000] 0 [01] 0 - 000000

            Não usam ALU:
              LUI
              JAL


        */
  
  src1 = regs[fields[rs1]].uval;
  src2 = (ctrl_reg >> 9) & 1 ? fields[imm] : regs[fields[rs2]].uval;  // ALU src (rs2 or imm)
  
  alu_res = alu(src1, src2, (uint8_t)((ctrl_reg >> 10) & 0x07));      // ALU opcode* [000]

  (ctrl_reg >> 13) & 1 ? (MEMORY[alu_res].sval = fields[rs2]) : (0);  // WE Mem

  databus = (ctrl_reg >> 14) & 1 ? MEMORY[alu_res].sval : alu_res;    // Final result from ALU or Mem (result src)
    
  (ctrl_reg >> 6) & 1 ? (regs[fields[rd]].sval = databus) : (0);      // WE register file 
    
  log_instr(instr, opcode, fields, regs);

  return 0;

}