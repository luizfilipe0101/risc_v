#ifndef DECODER_H
#define DECODER_H
#endif

#include <stdint.h>
#include "defines.h"

int check_type(int32_t instr, mem *flash, reg *regs, int16_t *pc);
