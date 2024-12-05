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

#define beq  0
#define bne  1
#define blt  4
#define bge  5
#define bltu 6
#define bgeu 7

#define addi 0

#define sub  0


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
