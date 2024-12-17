#ifndef DECODER_H
#define DECODER_H
#endif

#include <stdint.h>
#include "defines.h"

int check_type(int32_t instr, reg *regs, int16_t *pc);
extern uint32_t opcode;
extern int32_t fields[6];
