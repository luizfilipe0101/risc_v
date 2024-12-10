#include <stdint.h>

#ifndef DEFINES_H
#define DEFINES_H


#define rd    0
#define func3 1
#define rs1   2
#define rs2   3
#define imm   4
#define func7 5

#define itype 19
#define rtype 51
#define btype 99
#define ltype 3
#define utype 55
#define stype 35

#define lui 55
#define auipc 23
#define jal 110
#define jalr 0x67

#define beq  0x63
#define bne  0xE3
#define blt  0x263
#define bge  0x2E3
#define bltu 0x363
#define bgeu 0x3E3

#define lb 0x3
#define lh 0x83
#define lw 0x103
#define lbu 0x203
#define lhu 0x283

#define sb 0x23
#define sh 0xA3
#define sw 0x123

#define addi 0x13
#define slti 0x113
#define sltiu 0x193
#define xori 0x213
#define ori 0x313
#define andi 0x393
#define slli 0x93
#define srli 0x293
#define srai 0x8293

#define add
#define sub  0
#define sll
#define slt
#define sltu
#define xor
#define srl
#define sra
#define or
#define and


typedef union
{                     
  uint32_t uval;      
  int32_t  sval;      
}reg;   

typedef union
{
  uint8_t uval;
  int8_t  sval;
}mem;

#endif
