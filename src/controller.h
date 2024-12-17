/*
    PC target           ok
    Result source
    Memory write
    ALU control
    ALU Source
    ImmSource
    RegWrite
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdint.h>

extern int16_t ctrl_reg;
void ctrl_unit(uint32_t opcode, int32_t fnc3, int32_t fnc7);

#endif
