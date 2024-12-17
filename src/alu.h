#ifndef ALU_H
#define ALU_H
#endif

#include <stdint.h>
#include "defines.h"

int32_t alu(int32_t src1, int32_t src2, uint8_t ctrl);
void imediate(int32_t *fields, reg *regs);
void regist(int32_t *fields, reg *regs);
void load(int32_t *fields, reg *regs);
void upper(char opcode, int32_t *fields, reg *regs, uint16_t pc);
void store(reg *regs, int32_t *fields);
int16_t jump(uint32_t opcode, int32_t *fields, reg *regs, int16_t *pc);
