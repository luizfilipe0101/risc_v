#include <stdint.h>

#ifndef DEFINES_H
#define DEFINES_H

#define zr 0
#define ra 1
#define sp 2
#define gp 3
#define tp 4
#define x8 8
#define s0 8 
#define fp 8

#define a0 10
#define a1 11
#define a2 12
#define a3 13
#define a4 14
#define a5 15
#define a6 16
#define a7 17

#define s1 9
#define s2 18
#define s3 19
#define s4 20
#define s5 21
#define s6 22
#define s7 23
#define s8 24
#define s9 25
#define s10 26
#define s11 27

#define t0 5
#define t1 6
#define t2 7
#define t3 28
#define t4 29
#define t5 30
#define t6 31


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
#define jtype 111

#define lui 55
#define auipc 23
#define jal 111
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

#define ecall 115


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
