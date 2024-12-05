#ifndef ALU_H
#define ALU_H
#endif

#include <stdint.h>
#include "defines.h"

void imediate(int32_t *fields, reg *regs);
void regist(int32_t *fields, reg *regs);
void load(int32_t *fields, mem *flash, reg *regs);
void upper(int32_t *fields, reg *regs);
