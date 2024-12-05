#include "defines.h"
#include <stdint.h>

#ifndef LOG_H
#define LOG_H

int log_gen(char *rom, int romsize, mem *flash, reg *regs, uint32_t instr);
int log_instr(int32_t instr, uint32_t type, int32_t *fields, reg *regs);

#endif