/*
    PC target       0           0 = pc+4     1 = pc+target   
    
    Result source   1          00 = alu     01 = Mem
    Result source   1          10 = Imm     11 = ---

    Memory write    2           0 = False    1 = True

    ALU control     3         000 = Add    001 = Sub    010 = And  
    ALU control     3         011 = Or     100 = Slt    101 = sl
    ALU control     3         110 = sr

    ALU Source      4           0 = rs2      1 = Imm

    ImmSource       5         000 = I-type [31:20]      001 = S-type [31:25] [11:7]
    ImmSource       5         010 = U-type [31:12]      011 = J-type [31] [19:12] [20] [30:25] [24:21] [0]
    ImmSource       5

    RegWrite        6

    flag  negative  7
    flag  zero      7
    flag  carry     7
    flag  overflow  7

    0 [00] 0 [000] 0 [000] 0 [0000]
        
*/

#include <stdint.h>
#include <stdio.h>
#include "defines.h"

int16_t ctrl_reg = 0; 

void ctrl_unit(uint32_t opcode, int32_t fnc3, int32_t fnc7)
{
    switch(opcode)
    {
        case utype: /* 0x37 */
        /*
            PC target       0   
            
            Result source   1
            Result source   1

            Memory write    2

            ALU control     3
            ALU control     3
            ALU control     3

            ALU Source      4

            ImmSource       5
            ImmSource       5
            ImmSource       5

            RegWrite        6

            flag  negative  7
            flag  zero      7
            flag  carry     7
            flag  overflow  7

            0 [10] 0 [000] 0 [010] 1 [0000]
        */
            ctrl_reg = 0x4050;
        break;

        case itype:
            /*
            PC target       0   
            
            Result source   1
            Result source   1

            Memory write    2

            ALU control     3
            ALU control     3
            ALU control     3

            ALU Source      4

            ImmSource       5
            ImmSource       5
            ImmSource       5

            RegWrite        6
            zero            7

            0 [00] 0 [000] 0 [010] 1 0 000
        */
            switch(fnc3)
            {
                
            }
            ctrl_reg = 0x220;
        break;

        case ltype:
            // 010 [000] 1 [000] 1 - 00000
            // [000] = Sum
            ctrl_reg = 0x4220;
        break;

        case jal:
            /*
                PC target       0 = pc+4  --- 1 = pc(target)
                Result source   0 = ALU   --- 1 = Mem
                Memory write    0 = False --- 1 = True
                ALU control     [2:0] 0 = ?     --- 1 = ?       *** controlled by func3
                ALU Source      0 = rs2   --- 1 = Imm
                ImmSource       [1:0] 0 = ?     --- 1 = ?
                RegWrite        0 = False --- 1 = True
                ---             7
            */
            // 100 [000] 1 [11] 1 - 000000
            ctrl_reg = 0x83C0;
        break;

        case stype:
            /*
                PC target       0 = pc+4  --- 1 = pc(target)
                Result source   0 = ALU   --- 1 = Mem
                Memory write    0 = False --- 1 = True
                ALU control     [2:0] 0 = ?     --- 1 = ?
                ALU Source      0 = rs2   --- 1 = Imm
                ImmSource       [1:0] 0 = ?     --- 1 = ?
                RegWrite        0 = False --- 1 = True
                ---             7
            */
            // 001 [000] 1 [00] 0 - 000000
            ctrl_reg = 0xC2;
        break;
    }
}