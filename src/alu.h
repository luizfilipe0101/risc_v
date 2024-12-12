#ifndef ALU_H
#define ALU_H
#endif

#include <stdint.h>
#include "defines.h"

void imediate(int32_t *fields, reg *regs);
void regist(int32_t *fields, reg *regs);
void load(int32_t *fields, mem *flash, reg *regs);
void upper(char opcode, int32_t *fields, reg *regs, uint16_t pc);
void store(mem *flash, reg *regs, int32_t *fields);
int16_t jump(uint32_t opcode, int32_t *fields, reg *regs, int16_t *pc);
